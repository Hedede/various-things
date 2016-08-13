#!/usr/bin/bash

file=$1
form=$2

len=$((0x36))

head -c $len $file > head.bin
tail -c+$(($len+1)) $file > rest.bin

sox -t raw -c 1 -b 32 -e unsigned -r 8000 rest.bin -C 10 rest.$form
sox rest.$form -t raw -c 1 -b 32 -e unsigned -r 8000 rest.conv.bin
cat head.bin rest.conv.bin > ${1%.*}.${form}.bmp

rm rest.bin
rm rest.$form
rm head.bin
