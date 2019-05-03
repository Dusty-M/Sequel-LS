#ifndef SIGMAOPERATOR_H
#define SIGMAOPERATOR_H

#include "BaseTable.h"
#include "BaseOperator.h"
#include "SigmaBaseTable.h"
#include "SigmaBaseOperator.h"

class SigmaOperator: public BaseOperator {
private:
    BaseTblPtr parent_tbl;
    SigmaBaseTblPtr op_tree;
public:
    SigmaOperator(BaseTblPtr table, SigmaBaseTblPtr sigma_op_tree);
	virtual BaseTblPtr fabricate();	
};

#endif