#!/usr/bin/awk
match($0, /[0-9]+/) {
	print substr($0, RSTART, RLENGTH)
}
