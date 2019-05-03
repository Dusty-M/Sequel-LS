#include "RhoTableOperator.h"
#include "RhoTableTable.h"

RhoTableOperator::RhoTableOperator(BaseTblPtr table, std::string new_table_name) {
    parent_tbl = table;
    new_tbl_name = new_table_name;
}

BaseTblPtr RhoTableOperator::fabricate() {
    return make_rhotable_table(parent_tbl, new_tbl_name);
}