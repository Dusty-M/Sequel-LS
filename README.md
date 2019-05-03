To run the MSGdb program for regular input, follow the following steps:
Unzip MSGdb folder
Execute the following commands from the MSGdb folder:
    $./run_antlr.sh
    $make
    $bin/MSGdb
Execute queries separated by semicolons
Note that the table files that are created will be stored in the MSGdb folder. The program can also be tested by running multiple queries from a text file. This can be done by executing make test which executes a shell script, outlined in Section 7.0. 