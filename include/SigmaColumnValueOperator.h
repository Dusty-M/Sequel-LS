#ifndef SIGMACOLUMNVALUEOPERATOR_H
#define SIGMACOLUMNVALUEOPERATOR_H

#include <string>
#include <iostream>

#include "BaseTable.h"
#include "SigmaBaseOperator.h"

class SigmaColumnValueOperator: public SigmaBaseOperator {
private:
    std::string col_name;
public:
    SigmaColumnValueOperator(std::string col_name);
    virtual SigmaBaseTblPtr fabricate();
};

#endif 