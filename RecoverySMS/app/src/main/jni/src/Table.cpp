#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <memory>
#include <regex>

using namespace std;

#include "../include/CommonException.h"
#include "../include/StringFacility.h"

using namespace hudun::common;

#include "../include/Field.h"
#include "../include/Table.h"
#include "../include/Strto.h"

using namespace hudun::sqlite::schema;

#include <android/log.h>

#define TAG "scanning"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

Table::Table() noexcept {
}

Table::Table(string const &tableName) noexcept {
    this->tableName = tableName;
}

Table::Table(Table const &other) noexcept {
    this->tableName = other.tableName;
    this->fieldNameToIndexes = other.fieldNameToIndexes;
    this->fields = other.fields;
}

Table &Table::operator=(Table const &other) noexcept {
    if (this != &other) {
        this->tableName = other.tableName;
        this->fieldNameToIndexes = other.fieldNameToIndexes;
        this->fields = other.fields;
    }
    return *this;
}

bool Table::check() const noexcept {
    // check table tableName;
    if (this->tableName.empty() ||
        ((this->tableName.front() >= '0') && (this->tableName.front() <= '9')) ||
        (this->tableName.find_first_not_of(
                "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") != string::npos))
        return false;

    // check fields
    for (uint32_t i = 0; i < this->fields.size(); i++) {
        if (!this->fields[i].check()) return false;
    }

    return true;
}

string Table::getTableName() const noexcept {
    return this->tableName;
}

void Table::pushBack(Field const &field) {
    uint32_t fieldIndex = (uint32_t) this->fields.size();
    LOGI("fieldIndex %d === field.getFieldIndex = %d", fieldIndex, field.getFieldIndex());
    if (field.getFieldIndex() != fieldIndex) {
        throw CommonException(CommonException::FAIL_GENERAL,
                              "push_back index[" + to_string(fieldIndex) + "] != fieldIndex[" +
                              to_string(field.getFieldIndex()) +
                              "], please push_back field successively!", __FILE__, __func__,
                              __LINE__);
    }
    this->fields.push_back(field);
    this->fieldNameToIndexes.emplace(field.getFieldName(), fieldIndex);
}

uint32_t Table::getFieldNum() const noexcept {
    return (uint32_t) this->fields.size();
}

const Field &Table::getFieldByFieldIndex(uint32_t fieldIndex) const {
    if (fieldIndex >= this->fields.size()) {
        throw CommonException(CommonException::FAIL_GENERAL,
                              "Field Index[" + to_string(fieldIndex) + "] of Table[" +
                              this->tableName + "] overflow!", __FILE__, __func__, __LINE__);
    }
    return this->fields[fieldIndex];
}

const Field &Table::getFieldByFieldName(string const &fieldName) const {
    map<string, uint32_t>::const_iterator it = this->fieldNameToIndexes.find(fieldName);
    if (it == this->fieldNameToIndexes.end()) {
        throw CommonException(CommonException::FAIL_GENERAL,
                              "Field[" + fieldName + "] in Table[" + this->tableName +
                              "] not found!", __FILE__, __func__, __LINE__);
    }
    else {
        return this->getFieldByFieldIndex(this->fieldNameToIndexes.at(fieldName));
    }
}

vector<Field> Table::getPrimarykey() const noexcept {
    vector<Field> primaryKey;
    for (uint32_t i = 0; i < this->fields.size(); i++) {
        if (this->fields[i].isPrimaryKey()) {
            primaryKey.push_back(this->fields[i]);
        }
    }
    return primaryKey;
}

string Table::serialize() const noexcept {
    stringstream ss;
    ss << "CREATE TABLE " + this->tableName + " (\r\n";
    for (uint32_t i = 0; i < this->fields.size(); i++) {
        ss << "  " << this->fields[i].serialize("  ");
        if ((i + 1) == this->fields.size()) {
            ss << ");";
        }
        else {
            ss << ",\r\n";
        }
    }

    return ss.str();
}


static bool parseTableName(string const &ddl, string &tableName, string &rest) {
    const static regex tableNameRegex("^\\s*CREATE\\s+TABLE\\s+(\\w+)\\s*\\((.*)\\);*\\s*",
                                      regex_constants::ECMAScript | regex_constants::icase);
    smatch sm;
    bool b = regex_match(ddl, sm, tableNameRegex);
    LOGI("regex match %d \n sm size = %ld", b, sm.size());
    if (b && (sm.size() == 3)) {
        tableName = sm[1];
        LOGI("tableName = %s", tableName.c_str());
        rest = sm[2];
        LOGI("rest == %s", rest.c_str());
        return true;
    }
    else {
        return false;
    }
}

static bool isWhitespace(string const &s) {
    const static regex whitespaceRegex("^\\s*$");
    return regex_match(s, whitespaceRegex);
}

Table Table::parse(string const &ddl) {
    // �滻���лس�����
    regex crlf("[\\r|\\n]");
    string s = regex_replace(ddl, crlf, " ");
    LOGI(" s = %s", s.c_str());
    // tableName;
    string tableName;
    string rest;
    bool b = parseTableName(s, tableName, rest);
    LOGI("parse table name %d", b);
    if (!b) {
        LOGI("parse DDL : get table name");
        throw CommonException(CommonException::FAIL_GENERAL, "Parse DDL: getTableName[" + s + "]!",
                              __FILE__, __func__, __LINE__);
    }
    LOGI("TABLE NAME = %s", tableName.c_str());
    // ����table;
    Table table(tableName);

    // ����fields;
    uint32_t fieldIndex = 0;
    try {
        while (!isWhitespace(rest)) {
            shared_ptr<Field> field = Field::parse(rest, fieldIndex);

            if (field == nullptr) {
                LOGI("field is null");
                throw CommonException(CommonException::FAIL_GENERAL,
                                      "Parse DDL: field is nullptr[" + rest + "]!", __FILE__,
                                      __func__,
                                      __LINE__);
            }
            table.pushBack(*field);
            fieldIndex++;
        }

    } catch (CommonException &e) {
        LOGI("CommonException Exception, \nErrorCode:%lld, \nLine:%d, \nMsg: %s, \nDebugInfo:%s, \nFunctionName:%s",
             e.getCode(), e.getLineNo(), e.getMessage().c_str(), e.getDebugInfo().c_str(),
             e.getFunctionName().c_str());
    } catch (...) {
        LOGI("Unknown Exception");
    }

    return table;
}
