function showlink()
{
	ip addr show ppp0 2> /dev/null
}


if ! showlink
then
    pon workvpn
fi

while ! ip addr show ppp0 | grep -q '10.200.200... peer'; do
    sleep 1
done
ip route add 10.200.0.0/16 dev ppp0
