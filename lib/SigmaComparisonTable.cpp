#include "SigmaComparisonTable.h"

SigmaComparisonTable::SigmaComparisonTable(std::string comparator, SigmaBaseTblPtr left, SigmaBaseTblPtr right) {
    comp_op = comparator;
    left_child = left;
    right_child = right;
}

std::string SigmaComparisonTable::evaluate(std::vector<std::string> col_names, std::vector<std::string> row) {
    std::string left = left_child->evaluate(col_names, row);
    std::string right = right_child->evaluate(col_names, row);
    
    // Check for left equal to right.
    if (comp_op == "=" || comp_op == ">=" || comp_op == "<=") {
        if (left == right) {
            return "1";
        }
    }
    
    // Try to cast the left and right values to doubles.
    double left_dbl;
    double right_dbl;
    bool val_dbl = true; 

    try {
        left_dbl = stod(left);
        right_dbl = stod(right);
    }  catch (std::invalid_argument e) {
        val_dbl = false;   
    }

    // Check for left greater than right.
    if (comp_op == ">=" || comp_op == ">") {
        if ((val_dbl && left_dbl > right_dbl) || (!val_dbl && left > right)) {
            return "1";
        }
    }

    // Check for left less than right.
    if (comp_op == "<=" || comp_op == "<") {
        if ((val_dbl && left_dbl < right_dbl) || (!val_dbl && left < right)) {
            return "1";
        }
    } 

    return "0"; 
}

SigmaBaseTblPtr make_sigma_comparison_table(std::string comparator, SigmaBaseTblPtr left, SigmaBaseTblPtr right) {
    SigmaComparisonTable t1(comparator, left, right);
    return std::make_shared<SigmaComparisonTable> (t1);
}