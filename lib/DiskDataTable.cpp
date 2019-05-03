// DiskDataTable.cpp

#include "DiskDataTable.h"

// Constructor to load existing table from file
DiskDataTable::DiskDataTable(std::string filename) {
	table = DiskTableBackend::load(filename);
	row_reader = table->get_reader();
	table_name = filename.substr(0, filename.find(".table"));
}	

// Constructor to create new table on file
DiskDataTable::DiskDataTable(   std::string filename, 
				std::vector<std::string> col_names,
				std::vector<std::string> col_types) {
	try{
		DiskTableBackend::load(filename);
		// if code reaches this point, the load was successful,
		// which means the current create table statement should be aborted
		std::cout << "Table " << filename << " already exists, aborting create table statement" << std::endl;
		abort();
	} catch (DiskTableInvalidException e){
		// If there was a problem opening the file, code is redirected to this
		// catch block.  Therefore, create a new table as none of this name
		// already exist
		table = DiskTableBackend::create(filename, col_names, col_types);
		row_reader = table->get_reader();
		table_name = filename.substr(0, filename.find(".table"));
	}
}

// Constructor used by duplicate()
DiskDataTable::DiskDataTable(DiskTablePtr backend) {
	table = backend;
	row_reader = backend->get_reader();
}

std::vector<std::string> DiskDataTable::get_next() {
	return row_reader.get_next_row();
}

bool DiskDataTable::is_next() {
	return row_reader.has_next_row();
}

int DiskDataTable::get_num_cols() {
	return row_reader.get_num_columns();
}

int DiskDataTable::get_num_rows() {
	return row_reader.get_num_rows();
}

std::string DiskDataTable::get_tbl_name() const {
	return table_name;
}

std::vector<std::string> DiskDataTable::get_col_names() const {
	return table->get_column_names();
}

void DiskDataTable::insert_data_with_cols(	std::vector<std::string> columns, 
	std::vector<std::vector<std::string>> rows) {
	if (columns.size() != table->get_num_columns()) {
		std::cout << "Incorrect number of columns given.  Closing program" << std::endl;
		abort();
	}

	// Determine the order the values should be in when inserted into the table
	std::vector<int> indices{};
	std::vector<std::string> col_names = this->get_col_names();
	for(auto col_name : col_names) {
		int index = find(columns.begin(), columns.end(), col_name) - columns.begin();
		if (index >= int(columns.size())) {
			std::cout << "Invalid column name:" << col_name << " Closing program" << std::endl; 
			abort();
		} else {
			indices.push_back(index);
		}
	}

	// Check that each row contains the correct number of elements, then add each row to the table
	for (auto row: rows) {
		if (columns.size() != row.size()) {
			std::cout << "incorrect number of values given in a row.  Closing program" << std::endl;
			abort();
		} else {
			std::vector<std::string> result_values{};
			for (auto index : indices) {
				result_values.push_back(row.at(index));
			}
			// incremement row count and add row to .table file
			table->write_row(result_values);
		}
	}
}

void DiskDataTable::insert_data_without_cols(std::vector <std::vector<std::string> > rows) {
	unsigned int num_cols = table->get_num_columns();
	// ensure all rows have the correct number of elements
	// before inserting any rows (ensure atomicity of the statement)
	for( auto row : rows ) {
		if(num_cols != row.size()) {
			std::cout << "Wrong number of elements in row, closing program" << std::endl;
			abort();
		} else {
			table->write_row(row);
		}
	}
}

void DiskDataTable::reset() {
	row_reader = table->get_reader();
}


BaseTblPtr DiskDataTable::duplicate() const {
	return std::make_shared<DiskDataTable>(table);
}

BaseTblPtr make_disk_data_table(	
		std::string filename, 
		std::vector<std::string> column_names, 
		std::vector<std::string> column_types) {
	return std::make_shared<DiskDataTable>(filename, column_names, column_types);
}

BaseTblPtr make_disk_data_table(std::string filename) {
	return std::make_shared<DiskDataTable>(filename);
}
