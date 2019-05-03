// UnionAllOperator.h

#ifndef UNIONALLOPERATOR_H
#define UNIONALLOPERATOR_H

#include <vector>
#include <string>
#include "BaseTable.h"
#include "UnionAllTable.h"
#include "BaseOperator.h"

class UnionAllOperator: public BaseOperator {
private:
	BaseTblPtr left_parent_tbl, right_parent_tbl;
public:
	UnionAllOperator(BaseTblPtr left, BaseTblPtr right);
	virtual BaseTblPtr fabricate();
};

#endif
