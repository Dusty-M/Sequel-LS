#ifndef TABLE_TOOLS_H
#define TABLE_TOOLS_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include "BaseTable.h"
#include "DataTable.h"

void print_tbl_contents(BaseTblPtr table) {
	while (table->is_next()) {
		std::vector<std::string> row = table->get_next();
		for(auto element : row) {
			std::cout << std::left << std::setw(20) << element;
		}
		std::cout << std::endl;
	}
}


void store_tbl_contents(BaseTblPtr table, std::ofstream& outfile) {
	while (table->is_next()) {
		bool first = true;
		std::vector<std::string> row = table->get_next();
		for(auto element : row) {
			if(first){
				outfile << element;
			}else{
				outfile << "|" << element;
			}
			first = false;
		}
		outfile << std::endl;
	}
}


void print_col_names(std::vector<std::string> column_names) {
	for (auto element : column_names) {
		std::cout << std::left << std::setw(20) << element;
	}
	std::cout << std::endl;
	// Print a line to seperate column names from values.
	for (int i = 0; i < 20*int(column_names.size()); i++) {
		std::cout << "-";
	}
	std::cout << std::endl;	
}


void store_col_names(std::vector<std::string> column_names, std::ofstream& outfile) {
	bool first = true;
	for (auto element : column_names) {
		if(first){
			outfile << element;
		}else{
			outfile << "|" << element;
		}
		first = false;
	}
	outfile << std::endl;
}

#endif
