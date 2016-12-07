#!/usr/bin/bash
# Copyright (C) 2014  Hedede <hededrk@gmail.com>
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


print_nice_sub()
{
	fmt=$1
	val=$2

	[[ -z $fmt ]] && echo -n "$val" && return 1
	
	output='\e['

	bold=false
	dim=false
	underscore=false
	color=""
	backg=""

	local i=0
	while [[ $i<${#fmt} ]] ; do
		fm=${fmt:$i:1}
		case $fm in
			b)
				bold=true
				;;
			d)
				dim=true
				;;
			u)
				underscore=true
				;;
			K|R|G|O|B|M|C|W)
				if [[ -z $color ]] ; then
					color=$fm
				elif [[ -z $backg ]] ; then
					backg=$fm
				else
					echo "Only two colors can be specified" && return 1
				fi
		esac
		let i++
	done

	if [[ $bold = true ]] ; then
		output+='1;'
	fi
		
	if [[ $dim = true ]] ; then
		output+='2;'
	fi

	if [[ $underscore = true ]] ; then
		output+='4;'
	fi

	if [[ -n $color ]] ; then
		case $color in
			K)
				output+='30'
				;;
			R)
				output+='31'
				;;
			G)
				output+='32'
				;;
			O)
				output+='33'
				;;
			B)
				output+='34'
				;;
			M)
				output+='35'
				;;
			C)
				output+='36'
				;;
			W)
				output+='37'
				;;
		esac
		output+=';'
	fi

	if [[ -n $backg ]] ; then
		case $backg in
			K)
				output+='40'
				;;
			R)
				output+='41'
				;;
			G)
				output+='42'
				;;
			O)
				output+='43'
				;;
			B)
				output+='44'
				;;
			M)
				output+='45'
				;;
			C)
				output+='46'
				;;
			W)
				output+='47'
				;;
		esac
		output+=';'
	fi
	
	output=${output:0:-1}
	output+='m'
	output+=$2
	output+='\e[0m'

	echo -en $output
}

print_nice()
{
	while [[ ${#} -gt 1 ]]
	do
		print_nice_sub "$1" "$2"
		shift && shift
	done
	echo ''
}
