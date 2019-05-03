// DataTable.cpp - class used to store data

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "BaseTable.h"
#include "DataTable.h"

bool DataTable::is_next() {
	return counter < num_rows;
}

void DataTable::reset() {
	counter = 0;
}

BaseTblPtr DataTable::duplicate() const {
	return make_data_table(*this);
}

std::vector<std::string> DataTable::get_col_names() const {
	return col_names;
}

// This constructor is no longer used.  Replace when tables are made via sql insert statements
std::vector<std::string> DataTable::get_next() {
	return data.at(counter++);
}

int DataTable::get_counter() const {return counter;}
int DataTable::get_num_cols() const {return num_cols;}
int DataTable::get_num_rows() const {return num_rows;}
std::vector<std::vector<std::string> >  DataTable::get_data() const {return data;}
std::string DataTable::get_tbl_name() const {return tbl_name;}

void DataTable::insert_data_with_cols(std::vector<std::string> columns, std::vector<std::vector<std::string>> rows) {
	// Make sure the correct number of columns were provided.
	if (columns.size() != col_names.size()) {
		std::cout << "Incorrect number of columns given." << std::endl;
		abort();
	}

	// Make sure the correct number of values were provided for the number of given columns.
	for (auto row: rows) {
		if (columns.size() != row.size()) {
			std::cout << "Incorrect number of values given." << std::endl;
			abort();
		}
	}
	
	// Determine the order values should go into each row so they are in the same order as col_names.
	std::vector<int> indices{};
	for (auto col_name: col_names) {
		int index = find(columns.begin(), columns.end(), col_name) - columns.begin();
		if (index >= int(columns.size())) {
			std::cout << "Invalid column name: " << col_name << std::endl;
			abort();
		} else {
			indices.push_back(index);
		}           
	}

	// Add each row to the table.
	for (auto row: rows) {
		std::vector<std::string> result_values{};
		// Add the values to the result vector in the proper order. 
		for (auto index: indices) {
			result_values.push_back(row.at(index));
		}
		num_rows++;
		data.push_back(result_values);
	}
}

void DataTable::insert_data_without_cols(std::vector<std::vector<std::string>> rows) {
	// Make sure the correct number of values were provided for the number of given columns.
	for (auto row: rows) {
		if (col_names.size() != row.size()) {
			std::cout << "Incorrect number of values given." << std::endl;
			abort();
		}
	}

	// Insert the rows into data.
	for (auto row: rows) {
		num_rows++;
		data.push_back(row);
	}
}

// Not currently used
DataTable::DataTable() {
	counter = 0; // used to indicate the next row to return from get_next
	data = { {"ele1", "ele2", "ele3"}, {"ele4", "ele5", "ele6"} };
}

DataTable::DataTable(std::string table_name, std::vector<std::string> column_names) {
	counter = 0;
	tbl_name = table_name;
	col_names = column_names;
	data = {};
	num_cols = col_names.size();
	num_rows = 0;
}

DataTable::DataTable(const DataTable& table) {
	data = table.get_data();
	col_names = table.get_col_names();
	tbl_name = table.get_tbl_name();
	counter = table.get_counter();
	num_rows = table.get_num_rows();
	num_cols = table.get_num_cols();
}

DataTable::DataTable(int version){
	counter = 0;
	if (version == 1) {
		tbl_name = "products";
		col_names = {"product_id", "name", "price_kg"};
		data = {{"1", 		"Apple", 	"3.5"}, 
	  		{"2",		"Pear",		"4.0"},
			{"3",		"Lime",		"5.0"},
			{"4",		"Raspberry",	"10.0"},
			{"5",		"Peach",	"6.1"},
			{"6",		"Pineapple",	"1.75"}};
		num_cols = 3;
		num_rows = 6;
	} else if (version == 2) {
		tbl_name = "orders";
		col_names = {"order_num", "order_date", "customer_lastname", "customer_firstname"};
		data = {{"1000",	"2017-06-16",	"Bloom",		"Leopold"},
			{"1001",	"2017-07-05",	"Kafka",		"Franz"},
			{"1002",	"2017-10-17",	"Framboise",		"Fiona"},
			{"1003",	"2018-01-06",	"Hindbaer",		"Hannah"}};
		num_cols = 4;
		num_rows = 4;
	} else {
		tbl_name = "order_contents";
		col_names = {"order_num", "product_id", "kg_bought"};
		data = {{"1000", 	"3",		"0.6"},
			{"1001",	"3",		"10.0"},
			{"1001",	"5",		"2.5"},
			{"1001", 	"2",		"0.5"},
			{"1002",	"2",		"5.0"},
			{"1002",	"4",		"20.0"},
			{"1003",	"3",		"3.0"},
			{"1003",	"5",		"4.0"},
			{"1003",	"4",		"3.0"}};
		num_cols = 3;
		num_rows = 9;
	}
}

// This non-member function wraps a DataTable in a smart pointer.  
// Essentially this can be used as a constructor
BaseTblPtr make_data_table(int version) { 
//	DataTable t1(version);
//	return std::make_shared<DataTable> (t1);
	return std::make_shared<DataTable> (version);
}

BaseTblPtr make_data_table(DataTable t1) { 
	return std::make_shared<DataTable> (t1);
}

BaseTblPtr make_data_table(std::string table_name, std::vector<std::string> column_names) { 
	return std::make_shared<DataTable>(table_name, column_names);
}

