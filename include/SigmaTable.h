#ifndef SIGMATABLE_H
#define SIGMATABLE_H

#include <vector>
#include <string>
#include <iostream>         // std::cout
#include <algorithm>        // std::find
#include <stdexcept>

#include "BaseTable.h"
#include "SigmaBaseTable.h"

class SigmaTable: public BaseTable {
private:
    // Variables
    BaseTblPtr parent_tbl;
    SigmaBaseTblPtr op_tree;
    std::string tbl_name;
    bool is_next_row;
    std::vector<std::string> buffered_row;
public:
    // Constructor
    SigmaTable(BaseTblPtr table, SigmaBaseTblPtr sigma_op_tree);
    
    // Methods
    virtual bool is_next();
    virtual void reset();
    virtual BaseTblPtr duplicate() const;
    virtual std::vector<std::string> get_next();
    virtual std::vector<std::string> get_col_names() const;
    virtual std::string get_tbl_name() const;
};

BaseTblPtr make_sigma_table(BaseTblPtr table, SigmaBaseTblPtr sigma_op_tree);

#endif