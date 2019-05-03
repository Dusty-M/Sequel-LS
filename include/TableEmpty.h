// TableEmpty.h

#ifndef TABLEEMPTY_H
#define TABLEEMPTY_H

//#include <iostream>
//#include <iomanip>
//#include <vector>
//#include <string>
//#include <memory>
//#include "BaseTable.h"
//#include "DataTable.h"
#include <exception>

struct TableEmpty: public std::exception {
	const char* what () const throw () {
		return "TableEmpty exception";
	}
};

#endif
