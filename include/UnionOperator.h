// UnionOperator.h

#ifndef UNIONOPERATOR_H
#define UNIONOPERATOR_H

#include <vector>
#include <string>
#include "BaseTable.h"
#include "UnionTable.h"
#include "BaseOperator.h"

class UnionOperator: public BaseOperator {
private:
	BaseTblPtr left_parent_tbl, right_parent_tbl;
public:
	UnionOperator(BaseTblPtr left, BaseTblPtr right);
	virtual BaseTblPtr fabricate();
};

#endif
