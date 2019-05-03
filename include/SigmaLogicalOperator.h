#ifndef SIGMALOGICALOPERATOR_H
#define SIGMALOGICALOPERATOR_H

#include <string>

#include "SigmaBaseOperator.h"

class SigmaLogicalOperator: public SigmaBaseOperator {
private:
    std::string op;
    SigmaBaseTblPtr left_child;
    SigmaBaseTblPtr right_child;
public:
    SigmaLogicalOperator(std::string op_name, SigmaBaseTblPtr left, SigmaBaseTblPtr right);
    virtual SigmaBaseTblPtr fabricate();
};

#endif 