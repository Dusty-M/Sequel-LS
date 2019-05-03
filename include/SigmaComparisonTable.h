#ifndef SIGMACOMPARISONTABLE_H
#define SIGMACOMPARISONTABLE_H

#include <string>
#include <vector>

#include "SigmaBaseTable.h"

class SigmaComparisonTable: public SigmaBaseTable {
private:
    std::string comp_op;
    SigmaBaseTblPtr left_child;
    SigmaBaseTblPtr right_child;
public:
    SigmaComparisonTable(std::string comparator, SigmaBaseTblPtr left, SigmaBaseTblPtr right);    
    virtual std::string evaluate(std::vector<std::string> col_names, std::vector<std::string> row);
};

SigmaBaseTblPtr make_sigma_comparison_table(std::string comparator, SigmaBaseTblPtr left, SigmaBaseTblPtr right);

#endif