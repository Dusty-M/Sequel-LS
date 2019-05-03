#ifndef SIGMAARITHMETICOPERATOR_H
#define SIGMAARITHMETICOPERATOR_H

#include <string>

#include "SigmaBaseOperator.h"

class SigmaArithmeticOperator: public SigmaBaseOperator {
private:
    std::string op;
    SigmaBaseTblPtr left_child;
    SigmaBaseTblPtr right_child;
public:
    SigmaArithmeticOperator(std::string math_op, SigmaBaseTblPtr left, SigmaBaseTblPtr right);
    virtual SigmaBaseTblPtr fabricate();
};

#endif 