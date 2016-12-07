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

load_pkglist()
{
	unset pkglist
	unset pkglist_src
	unset pkglist_ver

	while IFS='|' read -r name source ver
	do
		pkglist+=("$name")
		pkglist_src+=("$source")
		pkglist_ver+=("$ver")
	done < "$repo_base/pkglist"
}

save_pkglist()
{
	mv "pkglist" "pkglist.old"

	for ((i=0; i<=${#pkglist[@]}; i++))
	do
		local name="${pkglist[$i]}"
		local src="${pkglist_src[$i]}"
		local ver="${pkglist_ver[$i]}"

		[[ -z "$name" ]] && continue
		printf '%s|%s|%s\n' "$name" "$src" "$ver"
	done > "$repo_base/pkglist"
}

pkglist_find()
{
	local pkgname=$1
	local i=0

	unset pkgid
	for pkg in ${pkglist[@]}
	do
		if [[ "$pkg" == "$pkgname" ]]
		then
			pkgid=$i
			return 0
		fi
		let i++
	done
	return 1
}

pkglist_remove()
{
	local pkgname=$1

	pkglist_find $pkgname || return 1

	unset pkglist[$pkgid]
	unset pkglist_src[$pkgid]
	unset pkglist_ver[$pkgid]
}

pkglist_add()
{
	local pkgname=$1
	local pkgsource=$2
	local pkgver=$3

	pkglist_find $pkgname && return 1

	pkglist+=("$pkgname")
	pkglist_src+=("$pkgsource")
	pkglist_ver+=("$pkgver")
}

pkglist_update()
{
	local pkgname=$1
	local pkgsource=$2
	local pkgver=$3

	pkglist_find $pkgname || return 1

	pkglist_src[$pkgid]=$pkgsource
	pkglist_ver[$pkgid]=$pkgver
}
