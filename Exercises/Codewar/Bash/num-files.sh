#!/bin/bash

[[ "$#" -lt 1 ]] && echo "Nothing to find" && exit 1
[[ ! -d "$1"  ]] && echo "Directory not found" && exit 1

echo "There are $(find "$1" -type f | wc -l) files in $(readlink -f "$1")"
