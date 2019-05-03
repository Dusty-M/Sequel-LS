#ifndef PIOPERATOR_H
#define PIOPERATOR_H

#include <vector>
#include <string>

#include "BaseTable.h"
#include "DataTable.h"
#include "PiTable.h"
#include "BaseOperator.h"

class PiOperator: public BaseOperator {
private:
    BaseTblPtr parent_tbl;
    std::vector<std::string> desired_cols;
public:
    PiOperator(BaseTblPtr table, std::vector<std::string> desired_columns);
    virtual BaseTblPtr fabricate();
};

#endif