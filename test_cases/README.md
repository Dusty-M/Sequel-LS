# The Testing Suite

## Testing Script: run\_tests.sh
run\_tests.sh is a testing script used to validate output of Sequel-LS.  It runs series of commands stored in text files through both Sequel-LS and SQLite and compares the outputs.

## test\_cases Directory Setup 
It is expected that there are a collection of text files in the test\_cases directory named test\*.txt.  (Examples: test1.txt, test01.txt, test955.txt)  The text files must contain valid SQLite statements.  

### General advice when writing the text files:
* The scripts should begin with 'drop if exist table' commands removing any tables being used by the test to ensure no preexisting table contents.
* A single line beginning with '--- ' should be used to briefly describe the test.  This will be displayed in the test results
* Like SQLite, '--' may be used to comment out lines within the text files.


## Running Tests
Note: run\_tests.sh must be run from the root directory (where the Makefile exists).

$ test\_cases/run\_tests.sh `<database program executable`>

For example: 
$ test\_cases/run\_tests.sh bin/Sequel-LS

A list will be displayed containing each .txt file tested, the description found within the text file, and either 'PASSED' or 'FAILED', depending on whether the outputs of Sequel-LS and SQLite matched.
