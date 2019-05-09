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
#include <unistd.h>

#include "DiskDataTable.h"

static void usage(void);

int main(int argc, char** argv) {
	std::string input_str;
	std::string input_file;
	std::string test_file;
	bool iflag = false; // Read from interactive mode by default.
	bool tflag = false; // Print output to screen by default.

	int ch;
	while ((ch = getopt(argc, argv, "i:t:")) != -1) {
		switch (ch) {
		case 'i': // Read from input file.
			iflag = true;
			input_file = optarg;
			break;
		case 't': // Write to test output file.
			tflag = true;
			test_file = optarg;
			break;
		default:
			usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (argc > 0) {
		usage();
	}

	if (iflag == true) { // Read from input file mode.
		std::ifstream infile;
		infile.open(input_file);
		if (!infile) {
			std::cerr << "Couldn't open input file" << std::endl;
			exit(1);
		}else{
			char c;
			while (infile.get(c)) {
				if(strcmp(&c, "\n")){
					input_str.push_back(c);
				}
			}
		}
	} else { // Interactive mode.
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
	if (tflag == true) { // Store output to file.
		std::ofstream outfile(test_file, std::ios::out | std::ios::trunc);
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

	std::cerr << "usage: " << __progname << " [-i infile] [-t outfile]" << std::endl;
	exit(1);
}
