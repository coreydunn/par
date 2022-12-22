#!/bin/sh

die()
{
	printf >&2 "$1"
}

assert()
{
	#local c_red="\[\033]31m\]"
	local c_red='\033[0;31m'
	local c_green='\033[0;32m'
	local c_yellow='\033[0;33m'
	local c_normal='\033[0;0m'

	printf "$c_yellow""$1""$c_normal"" => "
	if [ -e $1 ]; then
		$("./$1")
		local result="$?"
		if [ "$result" = 0 ]; then
			printf "$c_green""OK\n""$c_normal"
		else
			printf "$c_red""FAIL""$c_normal"" ($result)\n"
		fi
	else
		printf "$c_red""test not built\n""$c_normal"
	fi
}

main()
{
	cd $1
	for x in *.par; do
		assert "$(basename --suffix .par $x)"
	done
}

main $*