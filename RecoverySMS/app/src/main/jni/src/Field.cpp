#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <iostream>
#include <memory>
#include <regex>

using namespace std;

#include "../include/CommonException.h"
#include "../include/StringFacility.h"

using namespace hudun::common;

#include "../include/Field.h"
#include <android/log.h>

#define TAG "scanning"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
using namespace hudun::sqlite::schema;

static Field::TypeAffinity determineTypeAffinity(string const &declaredTypename) {
    string s = declaredTypename;
    StringFacility::toupper(s);

    // 1. If the declared type contains the string "INT" then it is assigned INTEGER affinity.
    size_t pos = s.find("INT");
    if (pos != string::npos) return Field::TypeAffinity::SQLITE_INTEGER;

    // 2. If the declared type of the column contains any of the strings "CHAR", "CLOB", or "TEXT" then that column has TEXT affinity.
    pos = s.find("CHAR");
    if (pos == string::npos) pos = s.find("CLOB");
    if (pos == string::npos) pos = s.find("TEXT");
    if (pos != string::npos) return Field::TypeAffinity::SQLITE_TEXT;

    // 3. If the declared type for a column contains the string "BLOB" .
    pos = s.find("BLOB");
    if ((pos != string::npos) || s.empty()) return Field::TypeAffinity::SQLITE_BLOB;

    // 4. If the declared type for a column contains any of the strings "REAL", "FLOA", or "DOUB" then the column has REAL affinity.
    pos = s.find("REAL");
    if (pos == string::npos) pos = s.find("FLOA");
    if (pos == string::npos) pos = s.find("DOUB");
    if (pos != string::npos) return Field::TypeAffinity::SQLITE_REAL;

    // 5. Otherwise, the affinity is NUMERIC.
    return Field::TypeAffinity::SQLITE_NUMERIC;
}

Field::Field(uint32_t fieldIndex, string const &fieldName, string const &declaredType) noexcept {
    this->fieldIndex = fieldIndex;
    this->fieldName = fieldName;
    this->declaredType = declaredType;
    this->typeAffinity = determineTypeAffinity(declaredType);
}

Field::Field(uint32_t fieldIndex, string const &fieldName, string const &declaredType,
             uint32_t typeLength) noexcept {
    this->fieldIndex = fieldIndex;
    this->fieldName = fieldName;
    this->declaredType = declaredType;
    this->typeAffinity = determineTypeAffinity(declaredType);
    this->typeLength = typeLength;
    this->flags |= Flag::TYPE_LENGTH;
}

Field::Field(uint32_t fieldIndex, string const &fieldName, string const &declaredType,
             uint32_t typeLength, uint32_t precisionLength) noexcept {
    this->fieldIndex = fieldIndex;
    this->fieldName = fieldName;
    this->declaredType = declaredType;
    this->typeAffinity = determineTypeAffinity(declaredType);
    this->typeLength = typeLength;
    this->precisionLength = precisionLength;
    this->flags |= Flag::TYPE_LENGTH;
    this->flags |= Flag::PRECISION_LENGTH;
}

Field::Field(Field const &other) noexcept {
    this->fieldIndex = other.fieldIndex;
    this->fieldName = other.fieldName;
    this->declaredType = other.declaredType;
    this->typeAffinity = other.typeAffinity;
    this->flags = other.flags;
    this->defaultValue = other.defaultValue;
    this->typeLength = other.typeLength;
    this->precisionLength = other.precisionLength;
    this->referenceTable = other.referenceTable;
    this->referenceField = other.referenceField;
}

Field &Field::operator=(Field const &other) noexcept {
    if (this != &other) {
        this->fieldIndex = other.fieldIndex;
        this->fieldName = other.fieldName;
        this->declaredType = other.declaredType;
        this->typeAffinity = other.typeAffinity;
        this->flags = other.flags;
        this->defaultValue = other.defaultValue;
        this->typeLength = other.typeLength;
        this->precisionLength = other.precisionLength;
        this->referenceTable = other.referenceTable;
        this->referenceField = other.referenceField;
    }
    return *this;
}

Field::~Field() {
}

void Field::setPrimaryKey() noexcept {
    this->flags |= Flag::PRIMARY_KEY;
}

void Field::setForeignKey() noexcept {
    this->flags |= Flag::FOREIGN_KEY;
}

void Field::setNotNull() noexcept {
    this->flags |= Flag::NOT_NULL;
}

void Field::setDefaultValue(string const &defaultValue) noexcept {
    this->flags |= Flag::DEFAULT_VALUE;
    this->defaultValue = defaultValue;
}

void Field::setUnique() noexcept {
    this->flags |= Flag::UNIQUE;
}

void Field::setReferences(string referenceTable, string referenceField) noexcept {
    this->flags |= Flag::REFERENCES;
    this->referenceTable = referenceTable;
    this->referenceField = referenceField;
}

void Field::setAutoIncrment() noexcept {
    this->flags |= Flag::AUTOINCREMENT;
}

bool Field::check() const noexcept {
    // check fieldName;
    if (this->fieldName.empty() ||
        ((this->fieldName.front() >= '0') && (this->fieldName.front() <= '9'))
        || (this->fieldName.find_first_not_of(
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") != string::npos))
        return false;

    // check declaredType;
    if (this->declaredType.empty()) return false;

    // check typeAffinity;
    if (!((this->typeAffinity == TypeAffinity::SQLITE_INTEGER) ||
          (this->typeAffinity == TypeAffinity::SQLITE_REAL) ||
          (this->typeAffinity == TypeAffinity::SQLITE_TEXT)
          || (this->typeAffinity == TypeAffinity::SQLITE_BLOB) ||
          (this->typeAffinity == TypeAffinity::SQLITE_NUMERIC)))
        return false;

    return true;
}

bool Field::isINTEGER() const noexcept {
    return ((this->typeAffinity & TypeAffinity::SQLITE_INTEGER) == TypeAffinity::SQLITE_INTEGER);
}

bool Field::isREAL() const noexcept {
    return ((this->typeAffinity & TypeAffinity::SQLITE_REAL) == TypeAffinity::SQLITE_REAL);
}

bool Field::isTEXT() const noexcept {
    return ((this->typeAffinity & TypeAffinity::SQLITE_TEXT) == TypeAffinity::SQLITE_TEXT);
}

bool Field::isBLOB() const noexcept {
    return ((this->typeAffinity & TypeAffinity::SQLITE_BLOB) == TypeAffinity::SQLITE_BLOB);
}

bool Field::isNUMERIC() const noexcept {
    return ((this->typeAffinity & TypeAffinity::SQLITE_NUMERIC) == TypeAffinity::SQLITE_NUMERIC);
}

bool Field::isPrimaryKey() const noexcept {
    return ((this->flags & Flag::PRIMARY_KEY) == Flag::PRIMARY_KEY);
}

bool Field::isForeignKey() const noexcept {
    return ((this->flags & Flag::FOREIGN_KEY) == Flag::FOREIGN_KEY);
}

bool Field::isNotNull() const noexcept {
    return ((this->flags & Flag::NOT_NULL) == Flag::NOT_NULL);
}

bool Field::hasDefaultValue() const noexcept {
    return ((this->flags & Flag::DEFAULT_VALUE) == Flag::DEFAULT_VALUE);
}

bool Field::isUnique() const noexcept {
    return ((this->flags & Flag::UNIQUE) == Flag::UNIQUE);
}

bool Field::isReferences() const noexcept {
    return ((this->flags & Flag::REFERENCES) == Flag::REFERENCES);
}

bool Field::isAutoIncrement() const noexcept {
    return ((this->flags & Flag::AUTOINCREMENT) == Flag::AUTOINCREMENT);
}

bool Field::hasTypeLength() const noexcept {
    return ((this->flags & Flag::TYPE_LENGTH) == Flag::TYPE_LENGTH);
}

bool Field::hasPrecisionLength() const noexcept {
    return ((this->flags & Flag::PRECISION_LENGTH) == Flag::PRECISION_LENGTH);
}

uint32_t Field::getFieldIndex() const noexcept {
    return this->fieldIndex;
}

string Field::getFieldName() const noexcept {
    return this->fieldName;
}

string Field::getDeclaredType() const noexcept {
    return this->declaredType;
}

Field::TypeAffinity Field::getTypeAffinity() const noexcept {
    return this->typeAffinity;
}

string Field::getDefaultValue() const {
    if ((this->flags & Flag::DEFAULT_VALUE) != Flag::DEFAULT_VALUE) {
        throw CommonException(CommonException::FAIL_GENERAL,
                              "Field[" + this->fieldName + "] no DEFAULT!", __FILE__, __func__,
                              __LINE__);
    }

    return this->defaultValue;
}

uint32_t Field::getTypeLength() const {
    if ((this->flags & Flag::TYPE_LENGTH) != Flag::TYPE_LENGTH) {
        throw CommonException(CommonException::FAIL_GENERAL,
                              "Field[" + this->fieldName + "] no TYPE_LENGTH!", __FILE__, __func__,
                              __LINE__);
    }

    return this->typeLength;
}

uint32_t Field::getPrecisionLength() const {
    if ((this->flags & Flag::PRECISION_LENGTH) != Flag::PRECISION_LENGTH) {
        throw CommonException(CommonException::FAIL_GENERAL,
                              "Field[" + this->fieldName + "] no PRECISION_LENGTH!", __FILE__,
                              __func__, __LINE__);
    }

    return this->precisionLength;
}

string Field::getReferenceTable() const {
    if ((this->flags & Flag::REFERENCES) != Flag::REFERENCES) {
        throw CommonException(CommonException::FAIL_GENERAL,
                              "Field[" + this->fieldName + "] no REFERENCES!", __FILE__, __func__,
                              __LINE__);
    }

    return this->referenceTable;
}

string Field::getReferenceField() const {
    if ((this->flags & Flag::REFERENCES) != Flag::REFERENCES) {
        throw CommonException(CommonException::FAIL_GENERAL,
                              "Field[" + this->fieldName + "] no REFERENCES!", __FILE__, __func__,
                              __LINE__);
    }

    return this->referenceField;
}

string Field::serialize(string prefix) const {
    // Prefix & fieldName;
    stringstream ss;
    ss << (prefix + this->fieldName);

    // declaredType;
    ss << " " << this->declaredType;

    if (this->hasTypeLength()) {
        ss << "(" << this->typeLength;
        if (this->hasPrecisionLength()) ss << "," << this->precisionLength;
        ss << ")";
    }

    // PRIMARY KEY;
    if (this->isPrimaryKey()) {
        ss << " PRIMARY KEY";
    }

    // FOREIGN KEY;
    if (this->isForeignKey()) {
        ss << " FOREIGN KEY";
    }

    // AUTOINCREMENT;
    if (this->isAutoIncrement()) {
        ss << " AUTOINCREMENT";
    }

    // NOT NULL;
    if (this->isNotNull()) {
        ss << " NOT NULL";
    }

    // DEFAULT VALUE;
    if (this->hasDefaultValue()) {
        ss << " DEFAULT " << this->defaultValue;
    }

    // UNIQUE;
    if (this->isUnique()) {
        ss << " UNIQUE";
    }

    // REFERENCE;
    if (this->isReferences()) {
        ss << " REFERENCES " + this->referenceTable + "(" + this->referenceField + ")";
    }

    return ss.str();
}

static bool parseToken(string &s, string &token) {
    LOGI("parseToken s %s", s.c_str());
    const static regex tokenRegex("^\\s*([\\w\\d]+)\\s*(.*)",regex_constants::ECMAScript);
    smatch sm;
    bool b = regex_match(s, sm, tokenRegex);

    LOGI("sm size = %ld,", sm.size());

    if (b && (sm.size() == 3)) {
        token = sm[1].str();
        s = sm[2].str();
        return true;
    }
    else {
        return false;
    }
}

static bool parseParameters(string &s, vector<string> &parameters) {
    const static regex tokenRegex("^\\s*\\(\\s*([\\s\\d\\w,]+)\\s*\\)\\s*(.*)",
                                  regex_constants::ECMAScript | regex_constants::icase);
    smatch sm;
    bool b = regex_match(s, sm, tokenRegex);
    if (b && (sm.size() == 3)) {
        parameters.clear();
        StringFacility::split(sm[1].str(), parameters, ", \t");
        s = sm[2].str();
        return true;
    }
    else {
        return false;
    }
}

static bool parseDefaultValue(string &s, string &defaultValue) {
    StringFacility::trim(s, StringFacility::WHITESPACE);
    if ((s[0] == '\'') || (s[0] == '\"')) {
        size_t n = s.find_first_of(s[0], 1);
        if (n == string::npos) {
            return false;
        }
        else {
            defaultValue = s.substr(0, n + 1);
            s = s.substr(n + 1);
            return true;
        }
    }
    else if (StringFacility::startsWith(s, "NULL", true)) {
        defaultValue = "NULL";
        s = s.substr(4);
        return true;
    }
    else {

        const static regex numericRegex("^\\s*([-|\\+]?[\\.\\d]+)\\s*(.*)");
        smatch sm;
        bool b = regex_match(s, sm, numericRegex);

        if (b && (sm.size() == 3)) {
            defaultValue = sm[1].str();
            s = sm[2];
            return true;
        }
        else {
            return false;
        }
    }
}

static bool isWhitespace(string const &s) {
    const static regex whitespaceRegex("^\\s*$");
    return regex_match(s, whitespaceRegex);
}

static bool isFieldEnd(string &s) {
    const static regex fieldEnd("^\\s*,\\s*(.*)",
                                regex_constants::ECMAScript | regex_constants::icase);
    smatch sm;
    bool b = regex_match(s, sm, fieldEnd);
    if (b && (sm.size() == 2)) {
        s = sm[1];
        return true;
    }
    else {
        return false;
    }
}

#ifdef _MSC_VER
#define __HUDUN_STRCASECMP__ _stricmp
#else
#define __HUDUN_STRCASECMP__ strcasecmp
#endif // _MSC_VER

#include <android/log.h>

#define TAG "scanning"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

shared_ptr<Field> Field::parse(string &rest, uint32_t fieldIndex) {
    // fieldName;
    string fieldName;
    bool b = parseToken(rest, fieldName);
    LOGI("parseToken is %d", b);
    if (!b) {
        LOGI("parseToken fail");
        throw CommonException(CommonException::FAIL_GENERAL, "Parse DDL: fieldName[" + rest + "]!",
                              __FILE__, __func__, __LINE__);
    }

    // fieldType;
    string declaredType;
    b = parseToken(rest, declaredType);
    if (!b) {
        throw CommonException(CommonException::FAIL_GENERAL,
                              "Parse DDL: declaredType[" + rest + "]!", __FILE__, __func__,
                              __LINE__);
    }

    // fieldType parameters;
    vector<string> parameters;
    shared_ptr<Field> field(nullptr);
    b = parseParameters(rest, parameters);
    if (!b) {
        field = shared_ptr<Field>(new Field(fieldIndex, fieldName, declaredType));
    }
    else {
        if (parameters.size() == 1) {
            uint32_t typeLength = (uint32_t) atoi(parameters[0].c_str());
            field = shared_ptr<Field>(new Field(fieldIndex, fieldName, declaredType, typeLength));
        }
        else if (parameters.size() == 2) {
            uint32_t typeLength = (uint32_t) atoi(parameters[0].c_str());
            uint32_t precisionLength = (uint32_t) atoi(parameters[1].c_str());
            field = shared_ptr<Field>(
                    new Field(fieldIndex, fieldName, declaredType, typeLength, precisionLength));
        }
        else {
            throw CommonException(CommonException::FAIL_GENERAL,
                                  "Parse DDL: getParameters[" + rest + "]!", __FILE__, __func__,
                                  __LINE__);
        }
    }

    while (!(isWhitespace(rest) || isFieldEnd(rest))) {
        string token;
        b = parseToken(rest, token);
        if (!b) {
            throw CommonException(CommonException::FAIL_GENERAL,
                                  "Parse DDL: getToken[" + rest + "]!", __FILE__, __func__,
                                  __LINE__);
        }

        // DEFAULT VALUE;
        if ((__HUDUN_STRCASECMP__(token.c_str(), "DEFAULT") == 0)) {
            string defaultValue;
            b = parseDefaultValue(rest, defaultValue);
            if (!b) {
                throw CommonException(CommonException::FAIL_GENERAL,
                                      "Parse DDL: getDefaultValue[" + rest + "]!", __FILE__,
                                      __func__, __LINE__);
            }
            field->setDefaultValue(defaultValue);
            continue;
        }

            // PRIMARY KEY;
        else if ((__HUDUN_STRCASECMP__(token.c_str(), "PRIMARY") == 0)) {
            b = parseToken(rest, token);
            if ((__HUDUN_STRCASECMP__(token.c_str(), "KEY") != 0)) {
                throw CommonException(CommonException::FAIL_GENERAL,
                                      "Parse DDL: PRIMARY 'KEY'[" + rest + "]!", __FILE__, __func__,
                                      __LINE__);
            }
            field->setPrimaryKey();
            continue;
        }

            // FOREIGN KEY;
        else if ((__HUDUN_STRCASECMP__(token.c_str(), "FOREIGN") == 0)) {
            b = parseToken(rest, token);
            if ((__HUDUN_STRCASECMP__(token.c_str(), "KEY") != 0)) {
                throw CommonException(CommonException::FAIL_GENERAL,
                                      "Parse DDL: FOREIGN 'KEY'[" + rest + "]!", __FILE__, __func__,
                                      __LINE__);
            }
            field->setForeignKey();
            continue;
        }

            // NOT NULL;
        else if ((__HUDUN_STRCASECMP__(token.c_str(), "NOT") == 0)) {
            b = parseToken(rest, token);
            if ((__HUDUN_STRCASECMP__(token.c_str(), "NULL") != 0)) {
                throw CommonException(CommonException::FAIL_GENERAL,
                                      "Parse DDL: NOT 'NULL'[" + rest + "]!", __FILE__, __func__,
                                      __LINE__);
            }
            field->setNotNull();
            continue;
        }


            // UNIQUE;
        else if ((__HUDUN_STRCASECMP__(token.c_str(), "UNIQUE") == 0)) {
            field->setUnique();
            continue;
        }

            // REFERENCES;
        else if ((__HUDUN_STRCASECMP__(token.c_str(), "REFERENCES") == 0)) {
            // referenceTable;
            string referenceTable;
            b = parseToken(rest, referenceTable);
            if ((!b)) {
                throw CommonException(CommonException::FAIL_GENERAL,
                                      "Parse DDL: referenceTable[" + rest + "]!", __FILE__,
                                      __func__, __LINE__);
            }

            // referenceField;
            b = parseParameters(rest, parameters);
            if (!b || (parameters.size() != 1)) {
                throw CommonException(CommonException::FAIL_GENERAL,
                                      "Parse DDL: referenceField[" + rest + "]!", __FILE__,
                                      __func__, __LINE__);
            }
            else {
                field->setReferences(referenceTable, parameters[0]);
            }
            continue;
        }

            // AUTOINCREMENT;
        else if ((__HUDUN_STRCASECMP__(token.c_str(), "AUTOINCREMENT") == 0)) {
            field->setAutoIncrment();
            continue;
        }

            // Others
        else {
            continue;
        }
    }
    return field;
}

#undef __HUDUN_STRCASECMP__
