#include "RhoColumnOperator.h"
#include "RhoColumnTable.h"

RhoColumnOperator::RhoColumnOperator(BaseTblPtr table, std::vector<std::string> new_column_name) {
    parent_tbl = table;
    new_col_name = new_column_name;
}

BaseTblPtr RhoColumnOperator::fabricate() {
    return make_rhocolumn_table(parent_tbl, new_col_name);
}