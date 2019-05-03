rm antlr_output/*
java -jar antlr-4.7.1-complete.jar -Dlanguage=Cpp -no-listener -visitor -o antlr_output SQL.g4
