#ifndef HUDUN_SQLITE_BASEAPI_SQLITEDAO_H
#define HUDUN_SQLITE_BASEAPI_SQLITEDAO_H
#include <string>
using namespace std;

#include "Entity.h"
#include "Incident.h"

namespace hudun
{
namespace sqlite
{
namespace baseapi
{
class SqliteDao
{
public:
    static void bindIntValue(string const& name, const hudun::sqlite::Entity* entity, void* statement, uint64_t attrFlag, int fieldIndex, int value, bool nullable, hudun::common::Incident& incident) noexcept;
    static void bindInt64Value(string const& name, const hudun::sqlite::Entity* entity, void* statement, uint64_t attrFlag, int fieldIndex, int64_t value, bool nullable, hudun::common::Incident& incident) noexcept;
    static void bindTextValue(string const& name, const hudun::sqlite::Entity* entity, void* statement, uint64_t attrFlag, int fieldIndex, string const& value, bool nullable, hudun::common::Incident& incident) noexcept;
};
}
}
}
#endif // HUDUN_SQLITE_BASEAPI_SQLITEDAO_H
