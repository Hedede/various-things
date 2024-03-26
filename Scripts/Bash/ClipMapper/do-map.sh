while IFS=$'\n' read -r command
do
	eval "./mapper.sh $command"
	tst=$?
	if [[ $tst == 3 ]]
	then
		false && echo "Warning: Link exists"
	else
	       	echo -e "./mapper.sh $command \e[1;31m[[$tst]]\e[0m"
	fi
done
