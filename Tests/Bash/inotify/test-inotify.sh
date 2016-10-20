#!/usr/bin/bash

dir="$HOME/hudd"
file="file.txt"
script="./script.sh"

while :
do
	inotifywait "$dir" -e close_write,moved_to  --format %e/%f |
	while IFS=/ read -r events mfile; do
		echo $mfile
		if [ "$file" = "$mfile" ]; then
			. $script
		fi
	done
done
