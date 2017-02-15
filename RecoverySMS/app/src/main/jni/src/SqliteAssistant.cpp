#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

#include "../include/CommonFacility.h"
#include "../include/Incident.h"
#include "../include/StringFacility.h"

using namespace hudun::common;

#include "../include/ColValue.h"
#include "../include/SqliteReturnCode.h"

using namespace hudun::sqlite;

#include "../include/ResultSet.h"
#include "../include/SqliteAssistant.h"

using namespace hudun::sqlite::baseapi;

#include "../include/sqlite3.h"
#include "../include/Strto.h"
#include <android/log.h>

#define TAG "scanning"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

void *SqliteAssistant::openDatabase(string const &dbFilename, Incident &incident) noexcept {
    // �����ݿ⣻
    sqlite3 *dbConnection;
    LOGI(" open database %s", dbFilename.c_str());
    int returnCode = sqlite3_open_v2(dbFilename.empty() ? nullptr : dbFilename.c_str(),
                                     &dbConnection, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                     nullptr);
    LOGI("open database result code %d", returnCode);
    if (returnCode != SQLITE_OK) {
        sqlite3_close(dbConnection);
        incident.set(SqliteReturnCode::FAIL_GENERAL, string("Open database") +
                                                     (dbFilename.empty() ? "[in Memory]: " : ("[" +
                                                                                              dbFilename +
                                                                                              "]: ")) +
                                                     sqlite3_errmsg(dbConnection), __FILE__,
                     __func__, __LINE__);
        return nullptr;
    }
    return (void *) dbConnection;
}

/*
void* SqliteAssistant::openDatabaseReadOnly(string const& dbFilename, Incident& incident) noexcept
{
    // ��ֻ����ʽ�����ݿ⣻
    sqlite3 *dbConnection;
    int returnCode = sqlite3_open_v2(dbFilename.c_str(), &dbConnection, SQLITE_OPEN_READONLY, nullptr);
    if(returnCode != SQLITE_OK)
    {
        sqlite3_close(dbConnection);
        incident.set(SqliteReturnCode::FAIL_GENERAL, string("Open database[") + dbFilename + "]: " + sqlite3_errmsg(dbConnection), __FILE__, __func__, __LINE__);
        return nullptr;
    }
    return (void*)dbConnection;
}
*/
void SqliteAssistant::closeDatabase(void *dbConnection) noexcept {
    sqlite3_close_v2((sqlite3 *) dbConnection);
}

void SqliteAssistant::query(void *dbConnection, uint32_t maxCount, string const &sql,
                            ResultSet &resultSet, Incident &incident) noexcept {
    // ����SQL���;
    sqlite3_stmt *statement = NULL;
    int returnCode = sqlite3_prepare_v2((sqlite3 *) dbConnection, sql.c_str(), -1, &statement,
                                        NULL);
    if (returnCode != SQLITE_OK) {
        incident.set(SqliteReturnCode::FAIL_GENERAL,
                     "Prepare statement[" + sql + "]: " + sqlite3_errmsg((sqlite3 *) dbConnection),
                     __FILE__, __func__, __LINE__);
        return;
    }

    // ����������
    resultSet.clear();
    uint32_t colNum = (uint32_t) sqlite3_column_count(statement);
    for (uint32_t colIndex = 0; colIndex < colNum; colIndex++) {
        const char *colName = sqlite3_column_name(statement, colIndex);
        string capColName(colName);
        resultSet.colNames.push_back(colName);

        StringFacility::toupper(capColName);
        resultSet.capColNameToIndexMap.emplace(capColName, colIndex);
    }

    // ִ��SQL��䣻
    vector<shared_ptr<ColValue>> row;
    while ((maxCount == 0) || (resultSet.getRowsNum() < maxCount)) {
        returnCode = sqlite3_step(statement);
        if (returnCode == SQLITE_ROW) {
            row.clear();
            for (uint32_t colIndex = 0; colIndex < colNum; colIndex++) {
                int colType = sqlite3_column_type(statement, colIndex);
                switch (colType) {
                    case SQLITE_NULL: {
                        row.push_back(shared_ptr<ColValue>(new ColValue()));
                    }
                        break;
                    case SQLITE_INTEGER: {
                        int64_t intValue = sqlite3_column_int64(statement, colIndex);
                        row.push_back(shared_ptr<ColValue>(new ColValue(intValue)));
                    }
                        break;
                    case SQLITE_FLOAT: {
                        double realValue = sqlite3_column_double(statement, colIndex);
                        row.push_back(shared_ptr<ColValue>(new ColValue(realValue)));
                    }
                        break;
                    case SQLITE_TEXT: {
                        const char *textValue = (const char *) sqlite3_column_text(statement,
                                                                                   colIndex);
                        row.push_back(shared_ptr<ColValue>(new ColValue(textValue)));
                    }
                        break;
                    case SQLITE_BLOB: {
                        int32_t blobLength = (int32_t) sqlite3_column_bytes(statement, colIndex);
                        const char *blob = (const char *) sqlite3_column_blob(statement, colIndex);
                        shared_ptr<char> blobBuff(new char[blobLength], default_delete<char[]>());
                        memcpy(blobBuff.get(), blob, blobLength);
                        row.push_back(shared_ptr<ColValue>(new ColValue(blobBuff, blobLength)));
                    }
                        break;
                    default:
                        incident.set(SqliteReturnCode::FAIL_GENERAL,
                                     "Unknown colType[" + to_string(colType) + "]!", __FILE__,
                                     __func__, __LINE__);
                        return;

                }
            }

            resultSet.rows.push_back(row);
            continue;
        }
        else if (returnCode == SQLITE_DONE) {
            break;
        }
        else {
            sqlite3_finalize(statement);
            incident.set(SqliteReturnCode::FAIL_GENERAL,
                         "Step return code[" + to_string(returnCode) + "]!", __FILE__, __func__,
                         __LINE__);
            return;
        }
    }

    sqlite3_finalize(statement);
    return;
}

vector<string> SqliteAssistant::queryTextValuesFromSingleField(void *dbConnection,
                                                               uint32_t maxCount, string const &sql,
                                                               Incident &incident) noexcept {
    // ����SQL��䣻
    sqlite3_stmt *statement = NULL;
    int returnCode = sqlite3_prepare_v2((sqlite3 *) dbConnection, sql.c_str(), -1, &statement,
                                        NULL);
    if (returnCode != SQLITE_OK) {
        incident.set(SqliteReturnCode::FAIL_GENERAL,
                     "Prepare statement[" + sql + "]: " + sqlite3_errmsg((sqlite3 *) dbConnection),
                     __FILE__, __func__, __LINE__);
        return vector<string>();
    }

    // ����е�������
    int columnCount = sqlite3_column_count(statement);
    if (columnCount != 1) {
        sqlite3_finalize(statement);
        incident.set(SqliteReturnCode::FAIL_GENERAL,
                     "Query columns[" + to_string(columnCount) + "] more than 1!", __FILE__,
                     __func__, __LINE__);
        return vector<string>();
    }

    // ִ��SQL��䣻
    vector<string> values;
    while ((maxCount == 0) || (values.size() < maxCount)) {
        returnCode = sqlite3_step(statement);
        if (returnCode == SQLITE_ROW) {
            string value = (char *) sqlite3_column_text(statement, 0);
            values.push_back(value);
            continue;
        }
        else if (returnCode == SQLITE_DONE) {
            break;
        }
        else {
            sqlite3_finalize(statement);
            incident.set(SqliteReturnCode::FAIL_GENERAL,
                         "Step return code[" + to_string(returnCode) + "]!", __FILE__, __func__,
                         __LINE__);
            return values;
        }
    }

    sqlite3_finalize(statement);
    return values;
}

void SqliteAssistant::executeSql(void *dbConnection, string const &sql,
                                 Incident &incident) noexcept {
    char *errorMessage = nullptr;
    incident.reset();
    int returnCode = sqlite3_exec((sqlite3 *) dbConnection, sql.c_str(), nullptr, nullptr,
                                  &errorMessage);
    if (returnCode != SQLITE_OK) {
        incident.set(SqliteReturnCode::FAIL_GENERAL, string(errorMessage) + "[" + sql + "]",
                     __FILE__, __func__, __LINE__);
        sqlite3_free(errorMessage);
        return;
    }
}
