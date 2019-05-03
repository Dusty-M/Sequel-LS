#ifndef BASETABLE_H
#define BASETABLE_H

#include <vector>
#include <string>
#include <memory>
class BaseTable; // fwd declaration of BaseTable for the purpose of BaseTblPtr typedef
typedef std::shared_ptr<BaseTable> BaseTblPtr;
class BaseTable {
public:
	static const int DISK_MODE = 1;
	virtual bool is_next() = 0;
	virtual void reset() = 0;
	virtual BaseTblPtr duplicate() const = 0;
	virtual std::vector<std::string> get_next() = 0;
	virtual std::vector<std::string> get_col_names() const = 0;	
	virtual std::string get_tbl_name() const = 0;
	virtual void insert_data_with_cols(std::vector<std::string> columns, std::vector<std::vector<std::string>> rows) {}
	virtual void insert_data_without_cols(std::vector<std::vector<std::string>> rows) {}
};

#endif
