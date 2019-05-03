#ifndef PITABLE_H
#define PITABLE_H

#include <vector>
#include <string>
#include <iostream>         // std::cout
#include <algorithm>        // std::find
#include <stdexcept>

#include "BaseTable.h"
#include "DataTable.h"

class PiTable: public BaseTable {
private:
    // Variables
    BaseTblPtr parent_tbl;
    bool all_columns;
    std::vector<std::string> desired_cols;
    std::vector<int> col_indices;
    std::string tbl_name;
    
    // Methods
    void set_col_indices();

public:
    // Constructor
    PiTable(BaseTblPtr table, std::vector<std::string> desired_columns);
    
    // Methods
    virtual bool is_next();
    virtual void reset();
    virtual BaseTblPtr duplicate() const;
    virtual std::vector<std::string> get_next();
    virtual std::vector<std::string> get_col_names() const;
    virtual std::string get_tbl_name() const;
};

BaseTblPtr make_pi_table(BaseTblPtr table, std::vector<std::string> desired_columns);

#endif
