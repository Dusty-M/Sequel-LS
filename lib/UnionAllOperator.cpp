// UnionAllOperator.cpp

#include "UnionAllOperator.h"
#include "UnionAllTable.h"

UnionAllOperator::UnionAllOperator(BaseTblPtr left, BaseTblPtr right) {
	left_parent_tbl = left;
	right_parent_tbl = right;
}

BaseTblPtr UnionAllOperator::fabricate() {
	return make_union_all_table(left_parent_tbl, right_parent_tbl);
}
