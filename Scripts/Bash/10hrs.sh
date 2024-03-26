if [[ -e 10hrs.txt ]]; then
	rm 10hrs.txt
fi

for i in {1..2250}; do
	echo -e "file '20140218 :: 2MyHWuuihBw.mp4'" >> 10hrs.txt 
done

ffmpeg -f concat -i 10hrs.txt -c copy '10 hours of xz11.mp4'

rm 10hrs.txt
