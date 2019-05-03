#include "SigmaTable.h"

SigmaTable::SigmaTable(BaseTblPtr table, SigmaBaseTblPtr sigma_op_tree) {
    parent_tbl = table;
    op_tree = sigma_op_tree;
    tbl_name = "SigmaTable"; // This will be replaced with a call to a random generator. 
}

bool SigmaTable::is_next() {  
    // Get the column names of the parent table to pass to evaluate.
    std::vector<std::string> col_names = parent_tbl->get_col_names();

    // Parse the sigma tree, using evaluate -> will result in true or false.
    // If evaluates to true, change is_next_row to true, return true, else do the process again.
    while (parent_tbl->is_next()) {
        buffered_row = parent_tbl->get_next();
         
        if (op_tree->evaluate(col_names, buffered_row) == "1") {
            is_next_row = true;
            return true;
        }
    }
    
    is_next_row = false;
    return false;
}

void SigmaTable::reset() {
    parent_tbl->reset();
}

BaseTblPtr SigmaTable::duplicate() const {
    BaseTblPtr new_parent = parent_tbl->duplicate();
    return make_sigma_table(new_parent, op_tree);
}

std::vector<std::string> SigmaTable::get_next() {
    if (is_next_row) {
        return buffered_row;
    }
    return {};
}

std::vector<std::string> SigmaTable::get_col_names() const {
    return parent_tbl->get_col_names();
}

std::string SigmaTable::get_tbl_name() const {
    return tbl_name;
}

// This non-member function wraps a DataTable in a smart pointer.  
// Essentially this can be used as a constructor
BaseTblPtr make_sigma_table(BaseTblPtr table, SigmaBaseTblPtr sigma_op_tree) { 
    SigmaTable t1(table, sigma_op_tree);
	return std::make_shared<SigmaTable> (t1);
}