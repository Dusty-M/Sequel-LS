#ifndef TABLE_TOOLS_H
#define TABLE_TOOLS_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include "BaseTable.h"
#include "DataTable.h"

void print_tbl_contents(BaseTblPtr table, bool tester, std::ofstream& file) {
	if(tester) {
 		while (table->is_next()) {
			std::vector<std::string> row = table->get_next();
			for(auto element : row) {
				std::cout << std::left << std::setw(20) << element;
			}
			std::cout << std::endl;
		}
	}else{
		while (table->is_next()) {
			bool first = true;
			std::vector<std::string> row = table->get_next();
			for(auto element : row) {
				if(first){
					file << element;
				}else{
					file << "|" << element;
				}
				first = false;
			}
			file << std::endl;
		}
	//	file.close(); 
	}
}

void print_col_names(std::vector<std::string> column_names, bool tester, std::ofstream& file) {
	if(tester){
		for (auto element : column_names) {
			std::cout << std::left << std::setw(20) << element;
		}
		std::cout << std::endl;
		// Print a line to seperate column names from values.
		for (int i = 0; i < 20*int(column_names.size()); i++) {
			std::cout << "-";
		}
		std::cout << std::endl;	
	}else{
		bool first = true;
		for (auto element : column_names) {
			if(first){
				file << element;
			}else{
				file << "|" << element;
			}
			first = false;
		}
		file << std::endl;
	}
	

}

#endif
