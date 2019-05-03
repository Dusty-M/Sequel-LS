//UnionAllTable.h

#ifndef UNIONALLTABLE_H
#define UNIONALLTABLE_H

#include <string>
#include <memory>
#include <vector>
#include "BaseTable.h"

class UnionAllTable: public BaseTable {
private:
	std::vector < std::string > col_names;
	BaseTblPtr left_parent, right_parent;
	std::string table_name;
public:
	virtual bool is_next();
	virtual std::vector < std::string > get_next();
	virtual std::vector < std::string > get_col_names() const;
	virtual std::string get_tbl_name() const;
	virtual void reset();
	virtual BaseTblPtr duplicate() const;
	UnionAllTable(BaseTblPtr left, BaseTblPtr right);
};
BaseTblPtr make_union_all_table(BaseTblPtr Left, BaseTblPtr Right);

#endif
