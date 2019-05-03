// UnionAllTable.cpp

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "BaseTable.h"
#include "UnionAllTable.h"

bool UnionAllTable::is_next() {
	return (left_parent->is_next() || right_parent -> is_next());
}

std::vector<std::string> UnionAllTable::get_next() {
	if(left_parent->is_next()) {
		return left_parent->get_next();
	} else if(right_parent->is_next()){
		return right_parent->get_next();
	} else {
		//std::cout << "ERROR - UnionAllTable::get_next() shouldn't have been run, there WAS no next" << std::endl;
		exit(0);
	}
}

std::vector <std::string>  UnionAllTable::get_col_names() const {
	return left_parent->get_col_names();
}

std::string UnionAllTable::get_tbl_name() const {
	return table_name;
}

void UnionAllTable::reset() {
	left_parent->reset();
	right_parent->reset();
}

BaseTblPtr UnionAllTable::duplicate() const {
	BaseTblPtr temp_right = right_parent->duplicate();
	BaseTblPtr temp_left = left_parent->duplicate();
	return make_union_all_table(temp_left, temp_right);
}

UnionAllTable::UnionAllTable(BaseTblPtr left, BaseTblPtr right) {
	if((left->get_col_names()).size() != (right->get_col_names()).size()) {
		std::cout << "Error - number of columns must match!  Invalid Query" << std::endl;
	} else {
		table_name = "UnionAll";
		//std::cout << "Number of columns match, generating resulting table" << std::endl;
		left_parent = left;
		right_parent = right;
	}
}

BaseTblPtr make_union_all_table(BaseTblPtr left, BaseTblPtr right) {
	return (std::make_shared<UnionAllTable> (left, right));
}

