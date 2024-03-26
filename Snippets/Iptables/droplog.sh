iptables -N LOG_DROP
iptables -A LOG_DROP -j LOG --log-prefix "INPUT:DROP: " --log-level 6
iptables -A LOG_DROP -j DROP
