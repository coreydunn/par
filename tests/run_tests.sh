#!/bin/sh

test_program()
{
	local c_red='\033[0;31m'
	local c_green='\033[0;32m'
	local c_yellow='\033[0;33m'
	local c_normal='\033[0;0m'

	# Remove color if in a pipe
	if [ ! -t 1 ]; then
		c_red=''
		c_green=''
		c_yellow=''
		c_normal=''
	fi

	# Run program and report result
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
		printf "$c_red""FAIL""$c_normal"" (not found)\n"
	fi
}

# Run tests on all specified programs
# Change directory if specified
main()
{
	[ $# -gt 0 ] && cd $1
	for x in *.par; do
		test_program "$(basename --suffix .par $x)"
	done
}

main $*
