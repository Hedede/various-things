COLS=$(tput cols)
ROWS=$(tput lines)
CLOCKWIDTH=8 #I'm assuming a HH:MM:SS format
CENTERCOL=$((COLS/2))
CENTERCOL=$((CENTERCOL-CLOCKWIDTH))
CENTERROW=$((ROWS/2))
tput cup $CENTERCOL $CENTERROW
echo $((1600000000 - $(date +%s)))
