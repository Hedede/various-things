#!/usr/bin/bash

[[ -z $1 ]] && exit 1
[[ -z $2 ]] && exit 1

to="$1"
shift
clips=("$@")
clips=(${clips[@]/#/clp})
clips_paths=( "${clips[@]/#/[unsorted]\/}" )

for ((i=0; i<${#clips_paths[@]}; i++))
do
	file=${clips_paths[$i]}
	[[ -e "${clips_paths[$i]}" ]] || clips_paths[$i]="${file%.*}.png"
	[[ -e "${clips_paths[$i]}" ]] || clips_paths[$i]="${file%.*}.jpg"
	[[ -e "${clips_paths[$i]}" ]] || clips_paths[$i]="${file%.*}.bmp"
	[[ -e "${clips_paths[$i]}" ]] || clips_paths[$i]="${file%.*}.ppm"
	[[ -e "${clips_paths[$i]}" ]] || clips_paths[$i]="${file%.*}.BMP"
	[[ -e "${clips_paths[$i]}" ]] || clips_paths[$i]="${file%.*}.JPG"
	if ! [[ -e "${clips_paths[$i]}" ]]
	then
		>&2 echo "${clips_paths[$i]} not found" && exit 2
	fi
done

mkdir -p $to

ln -srt "$to" "${clips_paths[@]}" 2>/dev/null || exit 3

info="$(printf "→ %s/%%s" $to)"
paste -d' ' <(printf "%s\n" "${clips_paths[@]}") <(printf "$info\n" "${clips[@]}")
