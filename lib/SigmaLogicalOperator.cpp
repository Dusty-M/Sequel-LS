#include "SigmaLogicalOperator.h"
#include "SigmaLogicalTable.h"

SigmaLogicalOperator::SigmaLogicalOperator(std::string op_name, SigmaBaseTblPtr left, SigmaBaseTblPtr right) {
    op = op_name;
    left_child = left;
    right_child = right;
}

SigmaBaseTblPtr SigmaLogicalOperator::fabricate() {
    return make_sigma_logical_table(op, left_child, right_child);
}