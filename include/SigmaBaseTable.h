#ifndef SIGMABASETABLE_H
#define SIGMABASETABLE_H

#include <string>
#include <vector>
#include <memory>

class SigmaBaseTable; // fwd decalration of SigmaBaseTable for the purpose of SigmaBaseTblPtr typedef
typedef std::shared_ptr<SigmaBaseTable> SigmaBaseTblPtr;
class SigmaBaseTable {
public:
    virtual std::string evaluate(std::vector<std::string> col_names, std::vector<std::string> row) = 0;
};

#endif