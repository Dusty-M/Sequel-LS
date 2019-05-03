#include "SigmaComparisonOperator.h"
#include "SigmaComparisonTable.h"

SigmaComparisonOperator::SigmaComparisonOperator(std::string comparator, SigmaBaseTblPtr left, SigmaBaseTblPtr right) {
    comp_op = comparator;
    left_child = left;
    right_child = right;
}

SigmaBaseTblPtr SigmaComparisonOperator::fabricate() {
    return make_sigma_comparison_table(comp_op, left_child, right_child);
}