#!/usr/bin/bash

ACTION=$1
GATEWAY=$2

setup_rules()
{
	local flag=$1

	#main
	iptables -t nat ${flag} PREROUTING -p tcp --dport 63330:63339 -j DNAT --to-destination ${GATEWAY%.*}.66
	iptables -t nat ${flag} PREROUTING -p udp --dport 63330:63339 -j DNAT --to-destination ${GATEWAY%.*}.66
	iptables -t nat ${flag} PREROUTING -p udp --dport 7877 -j DNAT --to-destination ${GATEWAY%.*}.66
	iptables ${flag} FORWARD -p tcp -d ${GATEWAY%.*}.66 --dport 63330:63339 -j ACCEPT
	iptables ${flag} FORWARD -p udp -d ${GATEWAY%.*}.66 --dport 63330:63339 -j ACCEPT
	iptables ${flag} FORWARD -p tcp -s ${GATEWAY%.*}.66 --dport 63330:63339 -j ACCEPT
	iptables ${flag} FORWARD -p udp -s ${GATEWAY%.*}.66 --dport 63330:63339 -j ACCEPT
	iptables ${flag} FORWARD -p udp -d ${GATEWAY%.*}.66 --dport 7877 -j ACCEPT
	iptables ${flag} FORWARD -p udp -s ${GATEWAY%.*}.66 --dport 7877 -j ACCEPT
}

start()
{
	ip addr flush enp1s0f3
	ip addr add ${GATEWAY%.*}.1 dev enp1s0f3

	#lan
	iptables -t nat -A POSTROUTING -o enp2s0 -j MASQUERADE
	iptables -A FORWARD -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
	iptables -A FORWARD -i enp1s0f3 -o enp2s0 -j ACCEPT

        DNS_PORT=5353
        iptables -w -I INPUT -p tcp -m tcp --dport $DNS_PORT -j ACCEPT
        iptables -w -I INPUT -p udp -m udp --dport $DNS_PORT -j ACCEPT
        iptables -w -t nat -I PREROUTING -s ${GATEWAY%.*}.0/24 -d ${GATEWAY} \
            -p tcp -m tcp --dport 53 -j REDIRECT --to-ports $DNS_PORT
        iptables -w -t nat -I PREROUTING -s ${GATEWAY%.*}.0/24 -d ${GATEWAY} \
            -p udp -m udp --dport 53 -j REDIRECT --to-ports $DNS_PORT
	iptables -I INPUT -p udp --dport 67 -i enp1s0f3 -j ACCEPT

	local flag2
	[[ $flag = -A ]] && flag2=-I || flag2=-D


	setup_rules -A

	mkdir /tmp/bullshito
	cat << EOF > /tmp/bullshito/dnsmasq.conf
listen-address=${GATEWAY}
bind-interfaces
dhcp-range=${GATEWAY%.*}.1,${GATEWAY%.*}.254,255.255.255.0,24h
dhcp-option-force=option:router,${GATEWAY}
dhcp-option-force=option:dns-server,${DHCP_DNS}
EOF
	MTU=$(get_mtu enp2s0)
	[[ -n "$MTU" ]] && echo "dhcp-option-force=option:mtu,${MTU}" >> /tmp/bullshito/dnsmasq.conf

	umask 0033
	dnsmasq -C /tmp/bullshito/dnsmasq.conf -x /tmp/bullshito/dnsmasq.pid -l /tmp/bullshito/dnsmasq.leases -p $DNS_PORT || die
	umask 0077

	ip link set enp1s0f3 up
}

stop()
{
	setup_rules -D

	iptables -t nat -D POSTROUTING -o enp2s0 -j MASQUERADE
	iptables -D FORWARD -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
	iptables -D FORWARD -i enp1s0f3 -o enp2s0 -j ACCEPT

	iptables -D INPUT -p udp --dport 67 -i enp1s0f3 -j ACCEPT
	iptables -D INPUT -p udp --dport 53 -s ${GATEWAY%.*}.0/24 -j ACCEPT
	iptables -D INPUT -p tcp --dport 53 -s ${GATEWAY%.*}.0/24 -j ACCEPT

	ip link set enp1s0f3 down
	ip addr flush enp1s0f3
}

if [[ $ACTION = stop ]]
then
	stop
elif [[ $ACTION = start ]]
then
	start
fi
