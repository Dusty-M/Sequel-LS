#include "SigmaLogicalTable.h"

SigmaLogicalTable::SigmaLogicalTable(std::string op_name, SigmaBaseTblPtr left, SigmaBaseTblPtr right) {
    op = op_name;
    left_child = left;
    right_child = right;
}

std::string SigmaLogicalTable::evaluate(std::vector<std::string> col_names, std::vector<std::string> row) {
    std::string left = left_child->evaluate(col_names, row);
    std::string right = right_child->evaluate(col_names, row);
    
    if (left == "1" && right == "1") {
        return "1";
    } else if ((left == "1" || right == "1") && (op == "or")) {
        return "1";
    } else {
        return "0";
    }
}

SigmaBaseTblPtr make_sigma_logical_table(std::string op_name, SigmaBaseTblPtr left, SigmaBaseTblPtr right) {
    SigmaLogicalTable t1(op_name, left, right);
    return std::make_shared<SigmaLogicalTable> (t1);
}