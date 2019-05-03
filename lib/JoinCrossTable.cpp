#include "JoinCrossTable.h"

JoinCrossTable::JoinCrossTable(BaseTblPtr table1, BaseTblPtr table2){
	table_1 = table1; 
	table_2 = table2;
	table_1_col_names = table_1->get_col_names();
	table_2_col_names = table_2->get_col_names();
	table_name = "CrossJoinTable";
	first_row = true;
}

bool JoinCrossTable::is_next() {
	return table_1->is_next() || table_2->is_next();
}

std::vector<std::string> JoinCrossTable::get_next() {
	std::vector<std::string> row;
	std::vector<std::string> table_2_row;

	if (first_row == true && table_1->is_next()) {
		table_1_row = table_1->get_next();
		first_row = false;
	}

	if (table_2->is_next()) {
		table_2_row = table_2->get_next();
		for(auto element : table_1_row) {
			row.push_back(element);
		}
		for(auto element : table_2_row) {
			row.push_back(element);
		}
		return row;
	} else {
		if(table_1->is_next()) {
			table_1_row = table_1->get_next();
			table_2->reset();
			table_2_row = table_2->get_next();
			for(auto element : table_1_row) {
				row.push_back(element);
			}
			for(auto element : table_2_row) {
				row.push_back(element);
			}
			return row;
		}
		return {};
	}

}

void JoinCrossTable::reset() {
	table_1->reset();
	table_2->reset();
}

BaseTblPtr JoinCrossTable::duplicate() const {
	BaseTblPtr temp_table1 = table_1->duplicate();
	BaseTblPtr temp_table2 = table_2->duplicate();
	return make_cross_join_table(temp_table1, temp_table2);
}

std::string JoinCrossTable::get_tbl_name () const {
	return table_name;
}

std::vector<std::string> JoinCrossTable::get_col_names() const { 
	std::vector<std::string> columns;
	
	for(auto element : table_1->get_col_names()){
		//columns.push_back(table_1->get_tbl_name() + "." + element);
		columns.push_back(element);

	}

	for(auto element : table_2->get_col_names()){
		//columns.push_back(table_2->get_tbl_name() + "." + element);
		columns.push_back(element);

	}

	return columns;
}

BaseTblPtr make_cross_join_table(BaseTblPtr table1, BaseTblPtr table2){
	JoinCrossTable t1 = JoinCrossTable(table1, table2);
	return std::make_shared<JoinCrossTable> (t1);
}
