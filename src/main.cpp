/* main.cpp 
   B. Bird - 10/24/2018
*/

#include "SQLComputeVisitor.h"
#include "SQLLexer.h"
#include "TableTools.h"
#include "TableDirectory.h"
#include <stdio.h>
#include <utility>
#include <string.h>

#include "DiskDataTable.h"

using std::string;


int main(int argc, char** argv) {
	string input_str;
	
	if(argv[1]){
		std::ifstream stdin;
		stdin.open(argv[1]);
		if(!stdin){
			std::cout << "Couldn't open input file" << std::endl;
			exit(1);
		}else{
			char c;
			while(stdin.get(c)){
				if(strcmp(&c, "\n")){
					input_str.push_back(c);
				}
			}
		}
	}else{
		std::cout << ">> ";
		std::getline(std::cin, input_str);
	}

	//The below was based on the ANTLR cpp official demo code
	antlr4::ANTLRInputStream input_stream(input_str);
	SQLLexer lexer(&input_stream);
	antlr4::CommonTokenStream tokens(&lexer);

	SQLParser parser(&tokens);
	antlr4::tree::ParseTree* tree = parser.primary_expression(); //The method is named "primary_expression" after the corresponding grammar rule

	// The next line can be uncommented to see how the parser parses the input statement.
	// std::cout << tree->toStringTree(&parser) << std::endl << std::endl;
	
	// Traversing tree with ComputeVisitor
	SQLComputeVisitor test;
	std::vector<BaseTblPtr> result = tree->accept(&test);
	
	std::ofstream file;
	file.open("./lib/testing/programoutput.csv", std::fstream::out | std::fstream::trunc);
	file.close();
	file.open("./lib/testing/programoutput.csv", std::fstream::out | std::fstream::app);

	if(strcmp(argv[2],"test")) {
		for (auto basetbl: result) {
			std::vector<std::string> col_names = basetbl->get_col_names();
			print_col_names(col_names, true, file);
			print_tbl_contents(basetbl, true, file);
		}
	}else{
		for (auto basetbl: result) {
			std::vector<std::string> col_names = basetbl->get_col_names();
			print_col_names(col_names, false, file);
			print_tbl_contents(basetbl, false, file);
		}
	}
	
	file.close();
}
