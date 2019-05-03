#ifndef SIGMACOMPARISONOPERATOR_H
#define SIGMACOMPARISONOPERATOR_H

#include <string>

#include "SigmaBaseOperator.h"

class SigmaComparisonOperator: public SigmaBaseOperator {
private:
    std::string comp_op;
    SigmaBaseTblPtr left_child;
    SigmaBaseTblPtr right_child;
public:
    SigmaComparisonOperator(std::string comparator, SigmaBaseTblPtr left, SigmaBaseTblPtr right);
    virtual SigmaBaseTblPtr fabricate();
};

#endif 