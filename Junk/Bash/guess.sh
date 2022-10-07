#!/usr/bin/bash
yuy=$(((RANDOM %1000)))
sudo beep -l 2000 -f $yuy

read guess

cmp1=guess
cmp2=yuy
if (( guess < yuy ))
then
	cmp1=yuy
	cmp2=guess
fi
if (( cmp1 - cmp2 < yuy / 8 ))
then
	echo KORREKT
else
	echo INKORREKT
fi

echo $yuy
