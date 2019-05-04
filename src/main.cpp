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

static void usage(void);

int main(int argc, char** argv) {
	string input_str;
	bool test_to_file = false; // Print output to screen by default.

	if (argc == 1) { // Interactive mode.
		std::cout << ">> ";
		std::getline(std::cin, input_str);
	} else if (argc > 1 && argc < 4) { // Read from file mode.
		// If 2nd arg is "test", then store output to file.
		if (argc == 3 && strcmp(argv[2], "test") == 0) {
			test_to_file = true;
		}

		std::ifstream stdin;
		stdin.open(argv[1]);
		if(!stdin){
			std::cerr << "Couldn't open input file" << std::endl;
			exit(1);
		}else{
			char c;
			while(stdin.get(c)){
				if(strcmp(&c, "\n")){
					input_str.push_back(c);
				}
			}
		}
	} else { // Not a valid mode.
		usage();
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

	if (test_to_file == true) { // Store output to file.
		std::ofstream outfile("./lib/testing/programoutput.csv",
			std::ios::out | std::ios::trunc);
		for (auto cur_tbl: result) {
			std::vector<std::string> col_names = cur_tbl->get_col_names();
			store_col_names(col_names, outfile);
			store_tbl_contents(cur_tbl, outfile);
		}
		outfile.close();
	} else { // Print output to screen.
		for (auto cur_tbl: result) {
			std::vector<std::string> col_names = cur_tbl->get_col_names();
			print_col_names(col_names);
			print_tbl_contents(cur_tbl);
		}
	}
}

static void usage(void)
{
	extern char *__progname;

	std::cerr << "usage: " << __progname << " [infile [test]]" << std::endl;;
	exit(1);
}
