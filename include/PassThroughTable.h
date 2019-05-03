#ifndef PASSTHROUGHTABLE_H
#define PASSTHROUGHTABLE_H

#include <vector>
#include <string>

#include "BaseTable.h"
#include "DataTable.h"


class PassThroughTable: public BaseTable {
private:
    // Variables
    BaseTblPtr parent_tbl;
    std::string tbl_name;

public:
    // Constructor
    PassThroughTable(BaseTblPtr table);
    
    // Methods
    virtual bool is_next();
    virtual void reset();
    virtual BaseTblPtr duplicate() const;
    virtual std::vector<std::string> get_next();
    virtual std::vector<std::string> get_col_names() const;
    virtual std::string get_tbl_name() const;
};

BaseTblPtr make_passthrough_table(BaseTblPtr table);

#endif
