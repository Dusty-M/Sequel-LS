#include "JoinNaturalOperator.h"
#include "JoinNaturalTable.h"

JoinNaturalOperator::JoinNaturalOperator(BaseTblPtr table1, BaseTblPtr table2){
	table_1 = table1;
	table_2 = table2;	
}

BaseTblPtr JoinNaturalOperator::fabricate(){
	return make_natural_join_table(table_1, table_2);
}

