#include "JoinNaturalTable.h"
#include "JoinCrossOperator.h"
#include "TableEmpty.h"

JoinNaturalTable::JoinNaturalTable(BaseTblPtr table1, BaseTblPtr table2){
	table_1 = table1; 
	table_2 = table2;
	table_1_col_names = table1->get_col_names();
	table_2_col_names = table2->get_col_names();
	check_mutual_columns();
	get_col_indices();
	table_name = "Natural_Join_Table"; //Hardcoded for now
	if(mutual_columns.empty()){
		JoinCrossOperator result(table_1, table_2);
		cross_join_ptr = result.fabricate();
	}
	first_row = true;
}

bool JoinNaturalTable::is_next() {
	if (mutual_columns.empty()) {
		return cross_join_ptr->is_next();
	} else {
		return is_next_helper();
	}
}

void JoinNaturalTable::get_col_indices(){
	for (auto col_name : mutual_columns) {
		int table_1_index = find(table_1_col_names.begin(), table_1_col_names.end(), col_name) - table_1_col_names.begin();
		int table_2_index = find(table_2_col_names.begin(), table_2_col_names.end(), col_name) - table_2_col_names.begin();
		table_1_col_indices.push_back(table_1_index);
		table_2_col_indices.push_back(table_2_index);
	} 
}

std::vector<std::string> JoinNaturalTable::get_next() {
	if (mutual_columns.empty()) {
		return cross_join_ptr->get_next();
	} else {
		return buffered_row;
	}
}

bool JoinNaturalTable::is_next_helper() {
	std::vector<std::string> table_2_row;
	bool equal_vals;
	buffered_row = {};
	while (buffered_row.empty()) {
		if (first_row == true && table_1->is_next()) {
			table_1->reset();
			table_1_row = table_1->get_next();
			first_row = false;
		}
		bool is_next_row_2 = table_2->is_next();
		while (is_next_row_2) {
			table_2_row = table_2->get_next();
			equal_vals = true;
			for(int i = 0; i < int(mutual_columns.size()); i++) {
				if(table_1_row[table_1_col_indices[i]] != table_2_row[table_2_col_indices[i]]) {
					equal_vals = false;
				}
			}
			if(equal_vals){
				for(auto element : table_1_row){
					buffered_row.push_back(element);
				}
				for(int j = 0; j < int(table_2_row.size()); j++) {
					int ind = find(table_2_col_indices.begin(), table_2_col_indices.end(), j) - table_2_col_indices.begin();
					if (ind >= int(table_2_col_indices.size())) {
						buffered_row.push_back(table_2_row[j]);
					}
				}
				return true;
			}
			is_next_row_2 = table_2->is_next();
		} 
		bool is_next_row = table_1->is_next();
		if(is_next_row) {
			table_1_row = table_1->get_next();
			table_2->reset();
		}
		if (!is_next_row && !is_next_row_2) {
			return false;
		}
	}
	return true;
}
          
BaseTblPtr JoinNaturalTable::duplicate() const {
	BaseTblPtr new_table_1 = table_1->duplicate();
	BaseTblPtr new_table_2 = table_2->duplicate();
	return make_natural_join_table(new_table_1, new_table_2);
}

std::vector<std::string> JoinNaturalTable::get_col_names() const { //needs to change
	if(mutual_columns.empty()){
		return cross_join_ptr->get_col_names();
	}else{
		std::vector<std::string> row;
		for(auto element : table_1->get_col_names()){
			row.push_back(element);
		}
		for(int j = 0; j < int(table_2->get_col_names().size()); j++) {
			int ind = find(table_2_col_indices.begin(), table_2_col_indices.end(), j) - table_2_col_indices.begin();
			if (ind >= int(table_2_col_indices.size())) {
				row.push_back(table_2->get_col_names()[j]);
			}
		}
		return row;
	}
}

void JoinNaturalTable::check_mutual_columns(){
	//check for mutual columns in table_1 and table_2
	for(auto element : table_1_col_names){
		if (std::find(std::begin(table_2_col_names), std::end(table_2_col_names), element) != std::end(table_2_col_names)){
			mutual_columns.push_back(element);
		}
	}
}

std::string JoinNaturalTable::get_tbl_name() const {
	return table_name;
}

void JoinNaturalTable::reset(){
	table_1->reset();
	table_2->reset();
}

BaseTblPtr make_natural_join_table(BaseTblPtr table1, BaseTblPtr table2){
	JoinNaturalTable t1(table1, table2);
	return std::make_shared<JoinNaturalTable> (t1);
}

