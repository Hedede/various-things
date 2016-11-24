re='^[0-9]+$'
for pid in /proc/*/
do
	pid=${pid#/*/}
	pid=${pid%/}
	if [[ $pid =~ $re ]] ; then
		echo $pid
		cat /proc/$pid/status | grep ctx
	fi
done
