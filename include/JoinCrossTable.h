#ifndef JOINCROSSTABLE_H
#define JOINCROSSTABLE_H

#include <vector>
#include <string>
#include <iostream>		//std::cout
#include <algorithm>	//std::find

#include "BaseTable.h"
#include "DataTable.h"

class JoinCrossTable: public BaseTable {
	private:
		//Variables
		BaseTblPtr table_1;
		BaseTblPtr table_2;

		bool first_row;

		std::string table_name;
		std::vector<std::string> table_1_row;
		std::vector<std::string> table_1_col_names;
		std::vector<std::string> table_2_col_names;		
		std::vector<std::string> mutual_columns;
		
	public:
		//Constructor
		JoinCrossTable(BaseTblPtr table1, BaseTblPtr table2);

		//Methods
		virtual bool is_next();
		virtual void reset();
		virtual BaseTblPtr duplicate() const;
		virtual std::vector<std::string> get_next();
		virtual std::vector<std::string> get_col_names() const;
		virtual std::string get_tbl_name() const;
};

BaseTblPtr make_cross_join_table(BaseTblPtr table1, BaseTblPtr table2);


#endif
