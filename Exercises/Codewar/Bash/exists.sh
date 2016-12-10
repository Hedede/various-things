#!/bin/bash

[[ -z "$1" ]] && echo "Nothing to find" && exit 1
[[ -e "$1" ]] && echo "Found $1" || echo "Can't find $1"
