#!/bin/bash
var1=$1
sqlite3 << EOF
.headers on
.output lib/testing/sqloutput.csv
.read $var1
EOF
bin/MSGdb $var1 test
if diff -q <(sort lib/testing/sqloutput.csv) <(sort lib/testing/programoutput.csv) &>/dev/null
then
	echo -e "Test:	\033[33;32mPassed\033[0m"
else
	echo -e "Test: \033[33;31mFailed\033[0m "
fi
