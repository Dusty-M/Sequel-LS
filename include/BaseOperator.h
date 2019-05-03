#ifndef BASEOPERATOR_H
#define BASEOPERATOR_H

#include "BaseTable.h"
#include "DataTable.h"

class BaseOperator {
public:
	virtual BaseTblPtr fabricate() = 0;	
};

#endif