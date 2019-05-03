#ifndef RHOTABLEOPERATOR_H
#define RHOTABLEOPERATOR_H

#include <vector>
#include <string>

#include "BaseTable.h"
#include "DataTable.h"
#include "BaseOperator.h"
#include "PassThroughTable.h"

class RhoTableOperator: public BaseOperator {
private:
    BaseTblPtr parent_tbl;
    std::string new_tbl_name;
public:
    RhoTableOperator(BaseTblPtr table, std::string new_tbl_name);
    virtual BaseTblPtr fabricate();
};

#endif