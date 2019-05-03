#ifndef JOINNATURALTABLE_H
#define JOINNATURALTABLE_H

#include <vector>
#include <string>
#include <iostream>		//std::cout
#include <algorithm>	//std::find

#include "BaseTable.h"
#include "DataTable.h"

class JoinNaturalTable: public BaseTable {
	private:
		//Variables
		BaseTblPtr table_1;
		BaseTblPtr table_2;
		std::string table_name;
		std::vector<int> table_1_col_indices;
		std::vector<int> table_2_col_indices;
		std::vector<std::string> table_1_col_names;
		std::vector<std::string> table_2_col_names;		
		std::vector<std::string> table_1_row;
		std::vector<std::string> mutual_columns;
		std::vector<std::string> buffered_row; 
		BaseTblPtr cross_join_ptr;
		bool first_row;
		
		//Methods
		void check_mutual_columns();
		void get_col_indices();
		bool is_next_helper();

		
	public:
		//Constructor
		JoinNaturalTable(BaseTblPtr table1, BaseTblPtr table2);

		//Methods
		virtual void reset();
		virtual bool is_next();
		virtual BaseTblPtr duplicate() const;
		virtual std::vector<std::string> get_next();
		virtual std::vector<std::string> get_col_names() const;
		virtual std::string get_tbl_name() const;

};

BaseTblPtr make_natural_join_table(BaseTblPtr table1, BaseTblPtr table2);

#endif
