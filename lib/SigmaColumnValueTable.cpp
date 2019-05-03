#include "SigmaColumnValueTable.h"

//SigmaColumnValueTable::SigmaColumnValueTable(BaseTblPtr table, std::string column_name) {
SigmaColumnValueTable::SigmaColumnValueTable(std::string column_name) {
    //tbl = table;
    //col_num = set_col_index(column_name);
    desired_col_name = column_name; 
}

std::string SigmaColumnValueTable::evaluate(std::vector<std::string> col_names, std::vector<std::string> row) {
    int col_num = set_col_index(col_names);
    return row.at(col_num);
}

int SigmaColumnValueTable::set_col_index(std::vector<std::string> col_names) {
    // Find the index of the column.
    int index = find(col_names.begin(), col_names.end(), desired_col_name) - col_names.begin();
    
    // The column name was not found. 
    if (index >= int(col_names.size())) {
        std::cout << "Invalid column name: " << desired_col_name << std::endl;
        abort();
    }      

    return index;
}

//SigmaBaseTblPtr make_sigma_columnvalue_table(BaseTblPtr table, std::string column_name) {
SigmaBaseTblPtr make_sigma_columnvalue_table(std::string column_name) {    
    //SigmaColumnValueTable t1(table, column_name);
    SigmaColumnValueTable t1(column_name);
    return std::make_shared<SigmaColumnValueTable> (t1);
}