// UnionOperator.cpp

#include "UnionOperator.h"
#include "UnionTable.h"

UnionOperator::UnionOperator(BaseTblPtr left, BaseTblPtr right) {
	left_parent_tbl = left;
	right_parent_tbl = right;
}

BaseTblPtr UnionOperator::fabricate() {
	return make_union_table(left_parent_tbl, right_parent_tbl);
}
