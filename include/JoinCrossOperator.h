#ifndef JOINCROSSOPERATOR_H
#define JOINCROSSOPERATOR_H

#include "BaseTable.h"
#include "DataTable.h"
#include "JoinCrossTable.h"
#include "BaseOperator.h"

class JoinCrossOperator {
	private:
		BaseTblPtr table_1;
		BaseTblPtr table_2;
	public:
		JoinCrossOperator(BaseTblPtr table1, BaseTblPtr table2);
		virtual BaseTblPtr fabricate();
	
};

#endif
