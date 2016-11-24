explode_args()
{
	unset OPTS
	local arg=$1
	while [[ $arg ]]
	do
		[[ $arg = '--' ]] && OPTS+=("$@") && break;
		if [[ ${arg:0:1} = '-' && ${arg:1:1} != '-' ]]
		then
			OPTS+=("${arg:0:2}")
			if (( ${#arg} > 2 )) 
			then
				arg="-${arg:2}" 
			else
				shift
				arg=$1
			fi
		else
			OPTS+=("$arg")
			shift
			arg=$1
		fi
	done
}
