#!/bin/bash

sqlite_out=sqlite_out.csv # output file used by sqlite3
prog_out=programoutput.csv # output file used by database engine being tested
#prog_name=${1?usage: run_test <path>/<db engine executable>}
if [ "X$1" = "X" ]; then # if there are no args
	echo "usage: run_tests <database engine executable being tested>"
	exit 1
fi
prog_name=$1

for filename in test_cases/test*.txt; do	
	# Find any line beginning with '---', then remove the first 4 characters from line
	test_descr=$(grep ^---.* $filename | cut -c 5-)

	$prog_name $filename test	# run database program in test mode
	if ! [ 0 -eq $? ] ; then # If exit status code != 0 then exit script
		echo "$prog_name failed to exit correctly, exit status is not zero.  Aborting script"
		exit 1
	fi

	# run sqlite3.  The usage of "<<- EOF ... EOF" allows the script to send multiple lines to
	# sqlite3 at once.
	# NOTE: the use of the '-' in '<<-' is required due to the text being indented
	sqlite3 <<- EOF
	.headers on
	.output $sqlite_out
	.read $filename
	EOF

	if ! [ 0 -eq $? ] ; then # If exit status code != 0 then exit script
		echo "sqlite3 failed to exit correctly, exit status is not zero.  Aborting script"
		exit 1
	fi

	# if script has reached this point, the input text file has been run through both programs
	# and there should be 2 .csv files containing their respective outputs
	# Check that both files exist before proceeding
	if [[ ! -f $sqlite_out ]] ; then
		echo "$sqlite_out does not exist.  Aborting"
		exit 1
	fi

	if [[ ! -f $prog_out ]] ; then
		echo "$prog_out does not exist.  Aborting"
		exit 1
	fi

	# sort and compare the outputs of both .csv files.
	# NOTE: sorting required since the outputs have no particular 
	if diff  <(sort $prog_out) <(sort $sqlite_out)>/dev/null; then
		printf "%-14s %-45s \033[33;32mPassed\033[0m\n" "$filename:" "$test_descr"
	else
		printf "%-14s %-45s \033[33;31mFailed\033[0m\n" "$filename:" "$test_descr"

	fi
	rm $prog_out && rm $sqlite_out 	# cleanup directory of temp .csv files
done
