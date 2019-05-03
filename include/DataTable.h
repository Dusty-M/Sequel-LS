// DataTable.h

#ifndef DATATABLE_H
#define DATATABLE_H

#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include "BaseTable.h"

class DataTable: public BaseTable {
private:
	std::vector<std::vector<std::string>> data;
	std::vector<std::string> col_names;
	std::string tbl_name;
	int counter, num_rows, num_cols;

	// Some private data getters
	int get_counter() const;
	int get_num_rows() const;
	int get_num_cols() const;
public:
	virtual bool is_next();
	virtual void reset();
	virtual BaseTblPtr duplicate() const;
	virtual std::vector<std::string> get_next();
	virtual std::vector<std::string> get_col_names() const;
	virtual std::string get_tbl_name() const;
	std::vector<std::vector<std::string> > get_data() const;
	virtual void insert_data_with_cols(std::vector<std::string> columns, std::vector<std::vector<std::string>> values);
	virtual void insert_data_without_cols(std::vector<std::vector<std::string>> values);
	DataTable();
	DataTable(std::string table_name, std::vector<std::string> column_names);
	DataTable(const DataTable& table);
	DataTable(int version);
};

BaseTblPtr make_data_table(int version);
BaseTblPtr make_data_table(DataTable t1);
BaseTblPtr make_data_table(std::string table_name, std::vector<std::string> column_names);

#endif
