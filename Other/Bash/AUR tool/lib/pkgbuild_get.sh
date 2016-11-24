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

source_pkgbuild()
{
	[[ -e $1 ]] || exit 1

	. $1
}

getver_()
{
	echo "$pkgver-$pkgrel"
}

getbase_()
{
	echo "$pkgbase"
}

is_split__()
{
	[[ "$(declare -p pkgname)" =~ "declare -a" ]] || return 1
}

is_split_()
{
	is_split__ && echo 1 || echo 0
}

getname_()
{
	is_split__ && echo ${pkgname[*]} || echo $pkgname
}

get_deps_()
{
	echo ${depends[*]}
}

get_makedeps_()
{
	echo ${makedepends[*]}
}

request_=$1
pkgname_=$2
if [[ -z $3 ]]
then
	pkgbuild_="build/${pkgname_}/PKGBUILD"
else
	pkgbuild_=$3
fi

source_pkgbuild "$pkgbuild_"

while [[ $i<${#request_} ]] ; do
	req=${request_:$i:1}
	case $req in
		v)
			getver_
			;;
		b)
			getbase_
			;;
		n)
			getname_
			;;
		s)
			is_split_
			;;
		d)
			get_deps_
			;;
		m)
			get_makedeps_
			;;
		
	esac
	let i++
done
