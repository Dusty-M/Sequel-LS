#ifndef SIGMACONSTANTVALUEOPERATOR_H
#define SIGMACONSTANTVALUEOPERATOR_H

#include <string>
#include <iostream>

#include "BaseTable.h"
#include "SigmaBaseOperator.h"

class SigmaConstantValueOperator: public SigmaBaseOperator {
private:
    std::string const_val;
public:
    SigmaConstantValueOperator(std::string const_value);
    virtual SigmaBaseTblPtr fabricate();
};

#endif 