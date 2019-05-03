#ifndef SIGMAARITHMETICTABLE_H
#define SIGMAARITHMETICTABLE_H

#include <string>
#include <vector>
#include <iostream>

#include "SigmaBaseTable.h"

class SigmaArithmeticTable: public SigmaBaseTable {
private:
    std::string op;
    SigmaBaseTblPtr left_child;
    SigmaBaseTblPtr right_child;
public:
    SigmaArithmeticTable(std::string math_op, SigmaBaseTblPtr left, SigmaBaseTblPtr right);    
    virtual std::string evaluate(std::vector<std::string> col_names, std::vector<std::string> row);
};

SigmaBaseTblPtr make_sigma_arithmetic_table(std::string math_op, SigmaBaseTblPtr left, SigmaBaseTblPtr right);

#endif