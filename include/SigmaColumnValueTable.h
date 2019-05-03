#ifndef SIGMACOLUMNVALUETABLE_H
#define SIGMACOLUMNVALUETABLE_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "BaseTable.h"
#include "SigmaBaseTable.h"

class SigmaColumnValueTable: public SigmaBaseTable {
private:
    BaseTblPtr tbl;
    std::string desired_col_name;
    int set_col_index(std::vector<std::string> col_names);
public:
    //SigmaColumnValueTable(BaseTblPtr table, std::string column_name);
    SigmaColumnValueTable(std::string column_name);
    virtual std::string evaluate(std::vector<std::string> col_names, std::vector<std::string> row);
};

//SigmaBaseTblPtr make_sigma_columnvalue_table(BaseTblPtr table, std::string column_name);
SigmaBaseTblPtr make_sigma_columnvalue_table(std::string column_name);

#endif 