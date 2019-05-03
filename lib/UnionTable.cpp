// UnionTable.cpp

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "BaseTable.h"
#include "UnionTable.h"
#include <exception>
#include "TableEmpty.h"
#include "DataTable.h"

bool UnionTable::is_next() {
	BaseTblPtr self_copy = this->duplicate();
	try {
		auto temp_row = self_copy->get_next(); 
	} catch(std::out_of_range& e) {
		return false;
	}catch(std::exception& TableEmpty) {
		return false;
	}
	return true; // if code executed this far, then there was a next in the self-copy
}

std::vector<std::string> UnionTable::get_next() {
	std::vector<std::string> cur_row;
	bool is_duplicate;
	while(left_parent->is_next()) {
		is_duplicate = false;
		cur_row = left_parent->get_next();
		for (auto element : cur_data) {
			if (element == cur_row) { // duplicate found, reject current row
				is_duplicate = true;
				break; 
			}
		}

		if (!is_duplicate) {
			cur_data.push_back(cur_row);
			return cur_row;
		}
	}
	// If code has executed this far, then the left_parent is 'depleted'
	while(right_parent->is_next()) {
		is_duplicate = false;
		cur_row = right_parent->get_next();
		for (auto element : cur_data) {
			if (element == cur_row) {  // duplicate found, reject cur_row
				is_duplicate = true;
				break;
			}
		}
		if(!is_duplicate) {
			cur_data.push_back(cur_row); // if no matches found, add cur_row to cur_data and return it
			return cur_row;
		}
	}
	// If code executes this far, then there is no next row to return, so raise exception
	throw TableEmpty();
}

std::vector <std::string>  UnionTable::get_col_names () const {
	return left_parent->get_col_names();
}

std::string UnionTable::get_tbl_name() const {
	return table_name;
}

void  UnionTable::set_data(std::vector <std::vector <std::string> > new_data) {
	cur_data = new_data;
}

std::vector <std::vector <std::string> > UnionTable::get_cur_data() {
	return cur_data;
}

void UnionTable::reset() {
	left_parent->reset();
	right_parent->reset();
	cur_data = {}; // resetting cur_data to empty set of rows
}

BaseTblPtr UnionTable::duplicate() const {
	BaseTblPtr left_dup = left_parent->duplicate();
	BaseTblPtr right_dup = right_parent->duplicate();
	BaseTblPtr the_copy = make_union_table(left_dup, right_dup);
	UnionTable* u_tbl_ptr;
	if((u_tbl_ptr = dynamic_cast<UnionTable*>((the_copy.get()))) == nullptr) {
		std::cout << "There's a problem with dyn. cast in UnionTable's duplicate()" << std::endl;
		exit(0);
	}
	u_tbl_ptr->set_data(cur_data);
	return the_copy;
}

UnionTable::UnionTable(BaseTblPtr left, BaseTblPtr right) {
	if((left->get_col_names()).size() != (right->get_col_names()).size()) {
		std::cout << "Error - number of columns must match!  Invalid Query" << std::endl;
	} else {
		table_name = "UnionTable";
		left_parent = left;
		right_parent = right;
	}
}

BaseTblPtr make_union_table(BaseTblPtr left, BaseTblPtr right) {
	return (std::make_shared<UnionTable> (left, right));
}
