#include "SigmaOperator.h"
#include "SigmaTable.h"

SigmaOperator::SigmaOperator(BaseTblPtr table, SigmaBaseTblPtr sigma_op_tree) {
    parent_tbl = table;
    op_tree = sigma_op_tree;
}

BaseTblPtr SigmaOperator::fabricate() {
    return make_sigma_table(parent_tbl, op_tree);
}