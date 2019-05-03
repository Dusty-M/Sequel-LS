#ifndef SIGMALOGICALTABLE_H
#define SIGMALOGICALTABLE_H

#include <string>
#include <vector>

#include "SigmaBaseTable.h"

class SigmaLogicalTable: public SigmaBaseTable {
private:
    std::string op;
    SigmaBaseTblPtr left_child;
    SigmaBaseTblPtr right_child;
public:
    SigmaLogicalTable(std::string op_name, SigmaBaseTblPtr left, SigmaBaseTblPtr right);    
    virtual std::string evaluate(std::vector<std::string> col_names, std::vector<std::string> row);
};

SigmaBaseTblPtr make_sigma_logical_table(std::string op_name, SigmaBaseTblPtr left, SigmaBaseTblPtr right);

#endif