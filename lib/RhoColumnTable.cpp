#include "RhoColumnTable.h"

RhoColumnTable::RhoColumnTable(BaseTblPtr table, std::vector<std::string> new_column_name) : PassThroughTable(table) {
    parent_tbl = table;
    new_col_name = new_column_name;
}
    
std::vector<std::string> RhoColumnTable::get_col_names() const {
    // Get current column names.
    std::vector<std::string> org_names = parent_tbl->get_col_names();
    std::vector<std::string> changed_names = {};

    // Initialize a flag to see if the name was changed.
    int changed = 0;

    // Find the old column name, create a new set of column names where this is replaced with the new name.
    for (auto name: org_names) {
        if (name == new_col_name.at(0)) {
            changed_names.push_back(new_col_name.at(1));
            changed = 1;
        } else {
            changed_names.push_back(name);
        }
    }

    if (!changed) {
        std::cout << "Invalid column name: " << new_col_name.at(0) << std::endl;
        // abort();
    }
    
    return changed_names;
}

BaseTblPtr make_rhocolumn_table(BaseTblPtr parent_table, std::vector<std::string> new_column_name) {
    RhoColumnTable t1(parent_table, new_column_name);
	return std::make_shared<RhoColumnTable> (t1);
}