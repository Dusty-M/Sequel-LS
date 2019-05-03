#ifndef RHOCOLUMNOPERATOR_H
#define RHOCOLUMNOPERATOR_H

#include <vector>
#include <string>

#include "BaseTable.h"
#include "DataTable.h"
#include "BaseOperator.h"
#include "PassThroughTable.h"

class RhoColumnOperator: public BaseOperator {
private:
    BaseTblPtr parent_tbl;
    std::vector<std::string> new_col_name;
public:
    RhoColumnOperator(BaseTblPtr table, std::vector<std::string> new_column_name);
    virtual BaseTblPtr fabricate();
};

#endif