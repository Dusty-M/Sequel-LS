#include "PassThroughTable.h"

PassThroughTable::PassThroughTable(BaseTblPtr table) {
    parent_tbl = table;
    tbl_name = "PassThroughTbl"; // Will be replaced with call to random name generator.
}

bool PassThroughTable::is_next() {
    return parent_tbl->is_next();
}

void PassThroughTable::reset() {
    parent_tbl->reset();
}

BaseTblPtr PassThroughTable::duplicate() const {
	return parent_tbl->duplicate();
}

std::vector<std::string> PassThroughTable::get_next() {
    return parent_tbl->get_next();
}

std::vector<std::string> PassThroughTable::get_col_names() const {
    return parent_tbl->get_col_names();
}

std::string PassThroughTable::get_tbl_name() const {
    return parent_tbl->get_tbl_name();
}

BaseTblPtr make_passthrough_table(BaseTblPtr table) { 
    PassThroughTable t1(table);
	return std::make_shared<PassThroughTable> (t1);
}
