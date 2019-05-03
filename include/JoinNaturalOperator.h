#ifndef JOINNATURALOPERATOR_H
#define JOINNATURALOPERATOR_H

#include "BaseTable.h"
#include "DataTable.h"
#include "JoinNaturalTable.h"
#include "BaseOperator.h"

class JoinNaturalOperator {
	private:
		BaseTblPtr table_1;
		BaseTblPtr table_2;
	public:
		JoinNaturalOperator(BaseTblPtr table1, BaseTblPtr table2);
		virtual BaseTblPtr fabricate();
};


#endif
