#!/usr/bin/bash

GATEWAY=$1
SUBNET=${GATEWAY%.*}
INET=enp2s0
WIFI=wlp4s0

ipchairs()
{
	iptables -w $@ || echo failed to $@
}

	echo Adding ethernet iface to bridge
	echo ip link set $WIFI up
	ip link set $WIFI up
	echo ip addr add ${GATEWAY}/24 broadcast ${SUBNET}.255 dev $WIFI
	ip addr add ${GATEWAY}/24 dev $WIFI

	echo Setting up NAT
	ipchairs -t nat -A POSTROUTING -o $INET -j MASQUERADE
	ipchairs -A FORWARD -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
	ipchairs -A FORWARD -i $WIFI -o $INET -j ACCEPT

	echo REDIRECT BULLSHIT
        DNS_PORT=5353
	iptables -A INPUT -p udp --dport 67 -i $WIFI -j ACCEPT
        ipchairs -t nat -A PREROUTING -s ${SUBNET}.0/24 -d ${GATEWAY} -p tcp -m tcp --dport 53 -j REDIRECT --to-ports $DNS_PORT
        ipchairs -t nat -A PREROUTING -s ${SUBNET}.0/24 -d ${GATEWAY} -p udp -m udp --dport 53 -j REDIRECT --to-ports $DNS_PORT
        ipchairs -A INPUT -p tcp -m tcp --dport $DNS_PORT -j ACCEPT
        ipchairs -A INPUT -p udp -m udp --dport $DNS_PORT -j ACCEPT

	echo CHECK ZEE BULSHIT
	iptables -w -L

	echo START DUNS MASQ
	dnsmasq -C /etc/dnsmasq-tmp.conf -p $DNS_PORT

	echo STRT HOST APED
	hostapd /etc/hostapd/hostapd.conf &
