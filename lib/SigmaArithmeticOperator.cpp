#include "SigmaArithmeticOperator.h"
#include "SigmaArithmeticTable.h"

SigmaArithmeticOperator::SigmaArithmeticOperator(std::string math_op, SigmaBaseTblPtr left, SigmaBaseTblPtr right) {
    op = math_op;
    left_child = left;
    right_child = right;
}

SigmaBaseTblPtr SigmaArithmeticOperator::fabricate() {
    return make_sigma_arithmetic_table(op, left_child, right_child);
}