#ifndef HUDUN_SQLITE_BASEAPI_SQLITEASSISTANT_H
#define HUDUN_SQLITE_BASEAPI_SQLITEASSISTANT_H
#include <string>
#include <map>
#include <vector>
using namespace std;

#include "Incident.h"
#include "ResultSet.h"

namespace hudun
{
namespace sqlite
{
namespace baseapi
{
class SqliteAssistant
{
public:
    static void* openDatabase(string const& dbFilename, hudun::common::Incident& incident) noexcept;
//    static void* openDatabaseReadOnly(string const& dbFilename, hudun::common::Incident& incident) noexcept;

    static void closeDatabase(void* dbConnection) noexcept;
    static void query(void* dbConnection, uint32_t maxCount, string const& sql, ResultSet& resultSet, hudun::common::Incident& incident) noexcept;
    static vector<string> queryTextValuesFromSingleField(void* dbConnection, uint32_t maxCount, string const& sql, hudun::common::Incident& incident) noexcept;

    static void executeSql(void* dbConnection, string const& sql, hudun::common::Incident& incident) noexcept;
};
}
}
}
#endif // HUDUN_SQLITE_BASEAPI_SQLITEASSISTANT_H
