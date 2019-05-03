#include "JoinCrossOperator.h"
#include "JoinCrossTable.h"

JoinCrossOperator::JoinCrossOperator(BaseTblPtr table1, BaseTblPtr table2){
	table_1 = table1;
	table_2 = table2;	
}

BaseTblPtr JoinCrossOperator::fabricate(){
	return make_cross_join_table(table_1, table_2);
}

