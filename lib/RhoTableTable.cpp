#include "RhoTableTable.h"

RhoTableTable::RhoTableTable(BaseTblPtr table, std::string new_table_name) : PassThroughTable(table) {
    parent_tbl = table;
    tbl_name = new_table_name;
}
    
std::string RhoTableTable::get_tbl_name() const {
    return tbl_name;
}

BaseTblPtr make_rhotable_table(BaseTblPtr parent_table, std::string new_table_name) {
    RhoTableTable t1(parent_table, new_table_name);
	return std::make_shared<RhoTableTable> (t1);
}