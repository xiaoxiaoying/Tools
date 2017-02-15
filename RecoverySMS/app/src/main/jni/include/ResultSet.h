#ifndef HUDUN_SQLITE_BASEAPI_RESULTSET_H
#define HUDUN_SQLITE_BASEAPI_RESULTSET_H
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

#include "ColValue.h"

namespace hudun
{
namespace sqlite
{
namespace baseapi
{
class SqliteAssistant;
class ResultSet
{
friend SqliteAssistant;
friend ostream& operator<<(ostream&, hudun::sqlite::baseapi::ResultSet const&) noexcept;
public:
    enum DisplayFlag: int
    {
        NONE       = 0,
        HORIZONTAL = 1
    };

protected:
    map<string, uint32_t> capColNameToIndexMap;
    vector<string> colNames;
    vector<vector<shared_ptr<hudun::sqlite::ColValue>>> rows;
public:
    uint32_t getColsNum() const noexcept;
    uint32_t getRowsNum() const noexcept;

    const vector<string>& getColNames() const;
    string getColName(uint32_t colIndex) const;

    const vector<shared_ptr<hudun::sqlite::ColValue>>& getRow(uint32_t rowIndex) const;
    shared_ptr<hudun::sqlite::ColValue> getValue(uint32_t rowIndex, uint32_t colIndex) const;
    shared_ptr<hudun::sqlite::ColValue> getValue(uint32_t rowIndex, string const& colName) const;


    vector<shared_ptr<hudun::sqlite::ColValue>> listColValues(uint32_t colIndex) const;
    vector<shared_ptr<hudun::sqlite::ColValue>> listColValues(string const& colName) const;

    void clear() noexcept;
    void displayColsNum(ostream&) const noexcept;
    void displayRow(ostream& out, vector<shared_ptr<hudun::sqlite::ColValue>> const& row, DisplayFlag displayFlag) const noexcept;
    void display(ostream& out, DisplayFlag displayFlag) const noexcept;
};
}
}
}
#endif // HUDUN_SQLITE_BASEAPI_RESULTSET_H
