#ifndef HUDUN_SQLITE_SCHEMA_TABLE_H
#define HUDUN_SQLITE_SCHEMA_TABLE_H

#include <string>
#include <vector>
#include <map>
using namespace std;

#include "Field.h"

namespace hudun
{
namespace sqlite
{
namespace schema
{
class Table
{
private:
    string tableName;
    map<string, uint32_t> fieldNameToIndexes;
    vector<Field> fields;
public:
    Table() noexcept;
    Table(string const& tableName) noexcept;
    Table(Table const&) noexcept;
    Table& operator=(Table const&) noexcept;

    bool check() const noexcept;

    string getTableName() const noexcept;

    void pushBack(Field const& field);

    uint32_t getFieldNum() const noexcept;
    const Field& getFieldByFieldIndex(uint32_t fieldIndex) const;
    const Field& getFieldByFieldName(string const& fieldName) const;
    vector<Field> getPrimarykey() const noexcept;

    string serialize() const noexcept;

    static Table parse(string const& ddl);
};
}
}
}
#endif // HUDUN_SQLITE_SCHEMA_TABLE_H
