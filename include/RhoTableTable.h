#ifndef RHOTABLETABLE_H
#define RHOTABLETABLE_H

#include <vector>
#include <string>

#include "BaseTable.h"
#include "DataTable.h"
#include "PassThroughTable.h"

class RhoTableTable: public PassThroughTable {
private:
    // Variables
    BaseTblPtr parent_tbl;
    std::string tbl_name;

public:
    // Constructor
    RhoTableTable(BaseTblPtr table, std::string new_table_name);
    
    // Methods
    virtual std::string get_tbl_name() const;
};

BaseTblPtr make_rhotable_table(BaseTblPtr parent_table, std::string new_table_name);

#endif


