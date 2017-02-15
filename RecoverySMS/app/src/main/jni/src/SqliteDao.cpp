#include <string>
using namespace std;

#include "../include/Incident.h"
using namespace hudun::common;

#include "../include/Entity.h"
#include "../include/SqliteReturnCode.h"
using namespace hudun::sqlite;

#include "../include/SqliteDao.h"
using namespace hudun::sqlite::baseapi;

#include "../include/sqlite3.h"

void SqliteDao::bindIntValue(string const& name, const Entity* entity, void* statement, uint64_t attrFlag, int fieldIndex, int value, bool nullable, Incident& incident) noexcept
{
    int returnCode;
    if(nullable && (!entity->isValid(attrFlag) || entity->isNull(attrFlag)))
    {
        returnCode = sqlite3_bind_null((sqlite3_stmt*)statement, fieldIndex);
    }
    else
    {
        returnCode = sqlite3_bind_int((sqlite3_stmt*)statement, fieldIndex, value);
    }
    if(returnCode != SQLITE_OK)
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "Bind[" + name + "]: " + sqlite3_errstr(returnCode), __FILE__, __func__, __LINE__);
    }
}

void SqliteDao::bindInt64Value(string const& name, const Entity* entity, void* statement, uint64_t attrFlag, int fieldIndex, int64_t value, bool nullable, Incident& incident) noexcept
{
    int returnCode;
    if(nullable && (!entity->isValid(attrFlag) || entity->isNull(attrFlag)))
    {
        returnCode = sqlite3_bind_null((sqlite3_stmt*)statement, fieldIndex);
    }
    else
    {
        returnCode = sqlite3_bind_int64((sqlite3_stmt*)statement, fieldIndex, (sqlite3_int64)value);
    }
    if(returnCode != SQLITE_OK)
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "Bind[" + name + "]: " + sqlite3_errstr(returnCode), __FILE__, __func__, __LINE__);
    }
}

void SqliteDao::bindTextValue(string const& name, const Entity* entity, void* statement, uint64_t attrFlag, int fieldIndex, string const& value, bool nullable, Incident& incident) noexcept
{
    int returnCode;
    if(nullable && (!entity->isValid(attrFlag) || entity->isNull(attrFlag)))
    {
        returnCode = sqlite3_bind_null((sqlite3_stmt*)statement, fieldIndex);
    }
    else
    {
        returnCode = sqlite3_bind_text((sqlite3_stmt*)statement, fieldIndex, value.c_str(), -1, SQLITE_TRANSIENT);
//        returnCode = sqlite3_bind_text64((sqlite3_stmt*)statement, fieldIndex, value.c_str(), value.length(), SQLITE_TRANSIENT, SQLITE_UTF8);
    }
    if(returnCode != SQLITE_OK)
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "Bind[" + name + "]: " + sqlite3_errstr(returnCode), __FILE__, __func__, __LINE__);
    }
}
