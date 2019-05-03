#ifndef TABLEDIRECTORY_H
#define TABLEDIRECTORY_H

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h> // for remove(), to delete file from disk
#include <cstdio>

#include "BaseTable.h"
#include "DiskDataTable.h"

class TableDirectory {
private:
    std::vector<BaseTblPtr> select_results;
    std::vector<BaseTblPtr> created_tables;
public:
    enum table_type {SELECT, CREATE, DROP, INSERT};
    void register_tbl(std::pair<table_type, BaseTblPtr> pr);
    bool does_tbl_exist(std::string tbl_name);
    BaseTblPtr get_tbl(std::string tbl_name, bool for_insert);
    void drop_table(std::string tbl_name);
    std::vector<BaseTblPtr> get_select_results();
};

#endif
