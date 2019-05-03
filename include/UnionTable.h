//UnionTable.h

#ifndef UNIONTABLE_H
#define UNIONTABLE_H

#include <string>
#include <memory>
#include <vector>
#include "BaseTable.h"
//#include "table_tools.h"
//#include <exception>

/*
struct TableEmpty : public std::exception {
	const char* what () const throw () {
		return "TableEmpty exception";
	}
}; 
*/

class UnionTable: public BaseTable {
private:
	std::vector < std::string > col_names;
	BaseTblPtr left_parent, right_parent;
	std::string table_name;
	
	// cur_data is a buffered version of the UnionTable, as such it's a 
	// collection of every get_next that's returned up to this point
	std::vector <std::vector <std::string> > cur_data;
public:
	virtual bool is_next();
	virtual std::vector < std::string > get_next();
	virtual std::vector < std::string > get_col_names() const;
	virtual std::string get_tbl_name() const;
	void set_data(std::vector <std::vector <std::string> >);
	std::vector <std::vector <std::string> > get_cur_data();

	virtual void reset();
	virtual BaseTblPtr duplicate() const;
	UnionTable(BaseTblPtr left, BaseTblPtr right);
};
BaseTblPtr make_union_table(BaseTblPtr Left, BaseTblPtr Right);

#endif
