#include "SigmaArithmeticTable.h"

SigmaArithmeticTable::SigmaArithmeticTable(std::string math_op, SigmaBaseTblPtr left, SigmaBaseTblPtr right) {
    op = math_op;
    left_child = left;
    right_child = right;
}

std::string SigmaArithmeticTable::evaluate(std::vector<std::string> col_names, std::vector<std::string> row) {
    std::string left = left_child->evaluate(col_names, row);
    std::string right = right_child->evaluate(col_names, row);

    // Try to cast the left and right values to doubles. If they are ints, they will also cast to doubles.
    double left_dbl;
    double right_dbl; 
    double result_dbl;

    try {
        left_dbl = stod(left);
        right_dbl = stod(right);
    }  catch (std::invalid_argument e) {
        std::cout << "Invalid arguments given: " << left << op << right << std::endl;   
        exit(0);
    }

    if (op == "+") {
        result_dbl = left_dbl + right_dbl;
    } else if (op == "-") {
        result_dbl = left_dbl - right_dbl;
    } else if (op == "*") {
        result_dbl = left_dbl * right_dbl;
    } else if (op == "/") {
        result_dbl = left_dbl / right_dbl;
    } else {
        std::cout << "Invalid arithmetic op given: " << op << std::endl;   
        exit(0);
    }

    // Convert the result to a string.
    std::string result = std::to_string(result_dbl);

    // The following two lines were taken from StackOverflow and are used to remove trailing zeros added by converting to string.
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    result.erase (result.find_last_not_of('.') + 1, std::string::npos);

    // Return the result.
    return result;
}

SigmaBaseTblPtr make_sigma_arithmetic_table(std::string math_op, SigmaBaseTblPtr left, SigmaBaseTblPtr right) {
    SigmaArithmeticTable t1(math_op, left, right);
    return std::make_shared<SigmaArithmeticTable> (t1);
}