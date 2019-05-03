#include "PiOperator.h"
#include "PiTable.h"

PiOperator::PiOperator(BaseTblPtr table, std::vector<std::string> desired_columns) {
    parent_tbl = table;
    desired_cols = desired_columns;
}

BaseTblPtr PiOperator::fabricate() {
    return make_pi_table(parent_tbl, desired_cols);
}