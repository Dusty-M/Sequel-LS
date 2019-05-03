#ifndef RHOCOLUMNTABLE_H
#define RHOCOLUMNTABLE_H

#include <vector>
#include <string>
#include <iostream>

#include "BaseTable.h"
#include "DataTable.h"
#include "PassThroughTable.h"

class RhoColumnTable: public PassThroughTable {
private:
    // Variables
    BaseTblPtr parent_tbl;
    std::vector<std::string> new_col_name;

public:
    // Constructor
    RhoColumnTable(BaseTblPtr table, std::vector<std::string> new_column_name);
    
    // Methods
    virtual std::vector<std::string> get_col_names() const;
};

BaseTblPtr make_rhocolumn_table(BaseTblPtr parent_table, std::vector<std::string> new_column_name);

#endif


