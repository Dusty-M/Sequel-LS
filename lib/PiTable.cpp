#include "PiTable.h"

PiTable::PiTable(BaseTblPtr table, std::vector<std::string> desired_columns) {
    parent_tbl = table;
    desired_cols = desired_columns;
    tbl_name = "PiTable"; // This will be replaced with a call to a random generator.
    set_col_indices();   
}

bool PiTable::is_next() {
    return parent_tbl->is_next();
}

void PiTable::reset() {
    parent_tbl->reset();
}

BaseTblPtr PiTable::duplicate() const {
	BaseTblPtr new_parent = parent_tbl->duplicate();
	return make_pi_table(new_parent, new_parent->get_col_names());
}

std::vector<std::string> PiTable::get_next() {
    if (all_columns) {
        return parent_tbl->get_next();
    } else {
        // Return the desired columns based on col_indices.
        std::vector<std::string> curr_row = parent_tbl->get_next();
        std::vector<std::string> row_values;

        // For each value in col_indices, get the value at that index in curr_row, and add it to row_values. 
        for (auto col_index: col_indices) {
            row_values.push_back(curr_row.at(col_index));
        }

        return row_values;
    }
}   

std::vector<std::string> PiTable::get_col_names() const {
    if (all_columns) {
        return parent_tbl->get_col_names();
    } else {
        return desired_cols;
    }    
}

std::string PiTable::get_tbl_name() const {
    return tbl_name;
}

void PiTable::set_col_indices() {
    // Get all the column names.
    std::vector<std::string> all_column_names = parent_tbl->get_col_names();

    if (desired_cols.at(0) == "*") {
        all_columns = true;
    } else {
        all_columns = false;
        
        // Compare the column names from desired_cols and determine the column indices in all_column_names.
        for (auto col_name: desired_cols) {
            int index = find(all_column_names.begin(), all_column_names.end(), col_name) - all_column_names.begin();
            if (index >= int(all_column_names.size())) {
                std::cout << "Invalid column name: " << col_name << std::endl;
                abort();
            } else {
                col_indices.push_back(index);
            }           
        }
    }
}

// This non-member function wraps a DataTable in a smart pointer.  
// Essentially this can be used as a constructor
BaseTblPtr make_pi_table(BaseTblPtr table, std::vector<std::string> desired_columns) { 
    PiTable t1(table, desired_columns);
	return std::make_shared<PiTable> (t1);
}

