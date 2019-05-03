// DiskDataTable.h

#ifndef DISKDATATABLE_H
#define DISKDATATABLE_H

#include "DiskTableBackend.h"
#include "BaseTable.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

class DiskDataTable : public BaseTable {
//class DiskDataTable {
private:
	DiskTablePtr table;
	DiskTableBackend::RowReader row_reader;
	std::string table_name;

public:

	// Constructor used by duplicate()
	DiskDataTable(DiskTablePtr backend);

	// Constructor used to create new tables on disk
	DiskDataTable(  std::string filename, 
			std::vector<std::string> col_names,
			std::vector<std::string> col_types); 

	// constructor to load existing table from disk
	DiskDataTable(std::string filename); 


	int get_num_cols();
	int get_num_rows();


	virtual bool is_next();
	virtual std::vector<std::string> get_next(); 
	virtual std::string get_tbl_name() const;
	virtual std::vector<std::string> get_col_names() const;
	virtual void insert_data_with_cols(	std::vector<std::string> columns,
					std::vector<std::vector<std::string>> rows);
	virtual void insert_data_without_cols(std::vector <std::vector <std::string> > rows);
	virtual void reset();
	virtual BaseTblPtr duplicate() const;
};

BaseTblPtr make_disk_data_table(
		std::string filename,
		std::vector<std::string> column_names,
		std::vector<std::string> column_types);

BaseTblPtr make_disk_data_table( std::string filename );

#endif
