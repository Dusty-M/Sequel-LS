#include "SigmaColumnValueOperator.h"
#include "SigmaColumnValueTable.h"

//SigmaColumnValueOperator::SigmaColumnValueOperator(BaseTblPtr table, std::string column_name) {
SigmaColumnValueOperator::SigmaColumnValueOperator(std::string column_name) {
    //tbl = table;
    col_name = column_name;
}

SigmaBaseTblPtr SigmaColumnValueOperator::fabricate() {
    //return make_sigma_columnvalue_table(tbl, col_name);
    return make_sigma_columnvalue_table(col_name);
}