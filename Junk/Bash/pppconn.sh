pon myppp
while ! ip addr show ppp0 | grep -q '10.0.10... peer'; do
    sleep 1
done
ip route add 10.0.10.0/16 dev ppp0
