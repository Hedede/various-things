#!/usr/bin/bash
RED='\033[0;31m'
NC='\033[0m'
function print_color {
   printf "${RED}${1}${NC}\n"
}
print_color 'zaz zoz'

