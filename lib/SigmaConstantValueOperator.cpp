#include "SigmaConstantValueOperator.h"
#include "SigmaConstantValueTable.h"

SigmaConstantValueOperator::SigmaConstantValueOperator(std::string const_value) {
    const_val = const_value;
}

SigmaBaseTblPtr SigmaConstantValueOperator::fabricate() {
    return make_sigma_constantvalue_table(const_val);
}