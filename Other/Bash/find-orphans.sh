find / \( -path '/sys' -o -path '/proc' -o -path '/home' -o -path '/var' \) -prune -o -type f -a -exec pacman -Qo '{}' \; | grep '^error:'
