# SEQUEL-LS

## What is it?
Sequel-LS is a database engine.  It is intended to match the functionality and syntax of SQLite.  Provided the database contents are the same, both database engines should have matching output given the same commands.  It should be noted that currently the project has only a small subset of operators and functionality present in SQLite.

This project was started as a Computer Science Directed Studies undergrad project at the University of Victoria.  It was completed by Jackie Gorman, Dusty Miller and Jaime Sedore under the supervision of Dr. Bill Bird during the fall term of 2018.

The goal of this project is to create a database engine with a set of functions and syntax that matches SQLite, but operates significantly faster than the alternative database engines by sacraficing the multi-user functionality of the other database engines.  By forgoing the overhead associated with maintaining ACID complience and instead simply removing the multiple concurrent user functionality, it is believed that Sequel-LS may be able to achieve significant gains in speed.

## How to Build and Test the Project
All commands below are to be executed from the root directory, where the Makefile is located.

### Build
$ make

### Run Tests
$ make test

Further details re: the testing suite can be found in the README in the test\_cases directory.

## How to Run the Project
$ bin/Sequel-LS \[-i `<input text file`>\] \[-t `<output filename`>\]
- If Sequel-LS is run without [-i infile] it allows for manual entry of commands.  
- If the -i (input) flag is used, the next argument must be the input text filename.
- If the -t (test) flag is used, the next argument must be the output filename.  The program will divert its output to the text file instead of printing to the screen.

## Dependencies
* Sequel-LS runs on Ubuntu 18.04 as well as the Linux kernel build into Windows 10 (Windows Subsystem for Linux).  
* Java v1.6 or higher. 
* C++ compiler that can handle C++11 or higher.
* GMake
* Antlr 4.7.1 (included in the repository)
* sqlite3

## Design Details
THIS SECTION HAS YET TO BE WRITTEN

## Known Bugs
* The Union operator currently crashes on many queries.  See section below for suggested fix.

## What's Next?
* Complete the 'Design Details' section of this document.
* Each source file needs to be combed through and edited for general style.  Removal of whitespace, removal of redundant/commented out blocks of code, etc.
* Debug/fix the Union operator.  It is crashing often.  The problem is suspected to arrise from an invalid duplication occuring in the is\_next() method.  This may be corrected by addressing the point below this, removing is\_next() from BaseTable class.
* Remove is\_next() from BaseTable class, change method of iterating tables.
    - At the start of this project it was policy to iterate through a table by checking if another row exists with is\_next(), and if true then retrieve the next row with get\_next().  
Towards the end of the project an operator was implemented that required the introduction of duplicate() to the BaseTable class.  Some operators require that tables be duplicated in order to run is\_next() as the function modifies the tables involved.  Since this is not acceptable behaviour of is\_next(), the tables must be duplicated, get\_next() is called on the duplicate in a try/catch block, and either a row is returned or an error is thrown.  Based on this result the is\_next() function can return either true or false.  
To avoid extremely poor performance when iterating through tables constructed by such operators, the overall policy on how tables are iterated through must change as follows:
    - remove is\_next() from the BaseTable interface
    - when iterating through tables, just call get\_next(), and put it it in a try/catch block
