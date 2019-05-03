#include "SigmaConstantValueTable.h"

SigmaConstantValueTable::SigmaConstantValueTable(std::string const_value) {
    const_val = const_value;
}

std::string SigmaConstantValueTable::evaluate(std::vector<std::string> col_names, std::vector<std::string> row) {
    return const_val;
}

SigmaBaseTblPtr make_sigma_constantvalue_table(std::string const_value) {
    SigmaConstantValueTable t1(const_value);
    return std::make_shared<SigmaConstantValueTable> (t1);
}