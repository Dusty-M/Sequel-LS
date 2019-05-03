#include "TableDirectory.h"

void TableDirectory::register_tbl(std::pair<table_type, BaseTblPtr> pr) {
    // Add the BaseTblPtr to the appropriate vector.
    switch(pr.first) {
        case SELECT: select_results.push_back(pr.second);
                     break;
        case CREATE: created_tables.push_back(pr.second);
                     break;
        case INSERT: break;
	case DROP:   break;
        default:     std::cout << "Invalid value in first element of pair.  Closing Program" << std::endl;
                     abort();
    }
}

// does_tbl_exist accounts for BaseTable::DISK_MODE, if appropriate it will check for the
// appropriate file on disk as part of this check.  If it does exist on disk it will automatically
// register it in tbl_dir.  
bool TableDirectory::does_tbl_exist(std::string tbl_name) {
	for (auto tbl: created_tables) {
		std::string curr_tbl_name = tbl->get_tbl_name();
		if (curr_tbl_name == tbl_name) {
			// The table already exists.
			return true;
		}
	}

	// The table is not currently registered in tbl_dir if code reaches this point
	switch(BaseTable::DISK_MODE) {
		case 0:
			return false;
			break;
		case 1:
			// try to load table
			try {
				BaseTblPtr result_tbl = make_disk_data_table(tbl_name + ".table");

				// if execution reaches this point, the file loaded.
				// so add the table to created_tables and return true
				created_tables.push_back(result_tbl);

				return true;
			} catch (DiskTableInvalidException e) {
				return false;
			}
			break;
		default:
			std::cout << "ERROR - BaseTable::DISK_MODE is neither 0 or 1.  Invalid DISK_MODE value currently set" << std::endl;
			std::cout << "Closing program" << std::endl;
			abort();
	}
}

BaseTblPtr TableDirectory::get_tbl(std::string tbl_name, bool for_insert) {
    BaseTblPtr result;
    for (auto tbl: created_tables) {
        std::string curr_tbl_name = tbl->get_tbl_name();
        if (curr_tbl_name == tbl_name) {
            if (for_insert) {
                return tbl;
            } else {
                result = tbl;
                break;
            }
        }
    }

    // Return a new BaseTblPtr to the table.
    return result->duplicate();
}

void TableDirectory::drop_table(std::string tbl_name) {
	unsigned int count = 0;
	for(auto element : created_tables) {
		if(element->get_tbl_name() == tbl_name) {
			// count is currently equal to position of table to be removed
			break;
		}
		count++;
	}
	if(count < created_tables.size()) { // indicates that a table with matching name was found
		created_tables.erase(created_tables.begin() + count);
	}

	// If BaseTable::DISK_MODE == 1, then also remove the file from disk
	switch (BaseTable::DISK_MODE) {
		case 0: // do nothing, the tbl_dir has already been updated
			break;
		case 1:
			{
			// delete file
			std::string filename = tbl_name + ".table";
			std::remove(filename.c_str());
			break;
			}
		default:
			std::cout << "ERROR - BaseTable::DISK_MODE is neither 0 or 1.  Invalid DISK_MODE value currently set" << std::endl;
			std::cout << "Closing program" << std::endl;
			abort();
	}
	return;
}

std::vector<BaseTblPtr> TableDirectory::get_select_results() {
    // Return all the select statement query results.
    return select_results;
}
