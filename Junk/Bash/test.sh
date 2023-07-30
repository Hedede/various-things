#!/usr/bin/bash

script="$HOME/script.sh"
filename="$HOME/file.txt"
datadir="$HOME/.cache/asdasd"
if ! [[ -e "$datadir" ]]
then
	mkdir -p $datadir
fi

if ! [[ -e "$datadir/ref" ]]
then
	touch "$datadir/ref"
fi

if [[ "$filename" -nt "$datadir/ref" ]]
then
	touch "$datadir/ref"
	exec $script
fi
