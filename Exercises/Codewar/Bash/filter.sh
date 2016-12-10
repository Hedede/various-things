#!/bin/bash

for file in *
do
	[[ ${file#*.} == "$1" ]] && echo $file
done
