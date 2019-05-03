#ifndef SIGMACONSTANTVALUETABLE_H
#define SIGMACONSTANTVALUETABLE_H

#include <string>
#include <vector>

#include "SigmaBaseTable.h"

class SigmaConstantValueTable: public SigmaBaseTable {
private:
    std::string const_val;
public:
    SigmaConstantValueTable(std::string const_value);
    virtual std::string evaluate(std::vector<std::string> col_names, std::vector<std::string> row);
};

SigmaBaseTblPtr make_sigma_constantvalue_table(std::string const_value);

#endif 