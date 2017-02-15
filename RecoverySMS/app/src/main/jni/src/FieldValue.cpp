#include <iostream>
#include <memory>
#include <string>
using namespace std;

#include "../include/CommonException.h"
#include "../include/CommonFacility.h"
#include "../include/StringFacility.h"
using namespace hudun::common;

#include "../include/FieldValue.h"
#include "../include/SqliteReturnCode.h"
#include "../include/SqliteFacility.h"
#include "../include/Strto.h"

using namespace hudun::sqlite;

FieldValue::FieldValue(uint32_t pageNumber, uint32_t fieldIndex, string const& fieldName)
{
    this->pageNumber = pageNumber;
    this->fieldIndex = fieldIndex;
    this->fieldName  = fieldName;
    this->fieldType  = 0;
    this->partial    = false;
}

FieldValue::FieldValue(uint32_t pageNumber, uint32_t fieldIndex, string const& fieldName, int64_t fieldType, uint32_t localOffset, uint32_t localSize, int64_t integerValue) noexcept
{
    this->pageNumber   = pageNumber;
    this->fieldIndex   = fieldIndex;
    this->fieldName    = fieldName;
    this->fieldType    = fieldType;
    this->localOffset  = localOffset;
    this->localSize    = localSize;
    this->integerValue = integerValue;
    this->partial      = false;
}

FieldValue::FieldValue(uint32_t pageNumber, uint32_t fieldIndex, string const& fieldName, int64_t fieldType, uint32_t localOffset, uint32_t localSize, double realValue) noexcept
{
    this->pageNumber  = pageNumber;
    this->fieldIndex  = fieldIndex;
    this->fieldName   = fieldName;
    this->fieldType   = fieldType;
    this->localOffset = localOffset;
    this->localSize   = localSize;
    this->realValue   = realValue;
    this->partial     = false;
}

FieldValue::FieldValue(uint32_t pageNumber, uint32_t fieldIndex, string const& fieldName, int64_t fieldType, uint32_t localOffset, uint32_t localSize, string const& textValue, bool partial) noexcept
{
    this->pageNumber  = pageNumber;
    this->fieldIndex  = fieldIndex;
    this->fieldName   = fieldName;
    this->fieldType   = fieldType;
    this->localOffset = localOffset;
    this->localSize   = localSize;
    this->textValue   = textValue;
    this->partial     = partial;
}

FieldValue::FieldValue(uint32_t pageNumber, uint32_t fieldIndex, string const& fieldName, int64_t fieldType, uint32_t localOffset, uint32_t localSize, shared_ptr<char> const& blobValue, bool partial) noexcept
{
    this->pageNumber  = pageNumber;
    this->fieldIndex  = fieldIndex;
    this->fieldName   = fieldName;
    this->fieldType   = fieldType;
    this->localOffset = localOffset;
    this->localSize   = localSize;
    this->blobValue   = pair<shared_ptr<char>, uint32_t>(blobValue, localSize);
    this->partial     = partial;
}

FieldValue::FieldValue(FieldValue const& other) noexcept
{
    this->pageNumber = other.pageNumber;
    this->fieldIndex = other.fieldIndex;
    this->fieldName = other.fieldName;
    this->fieldType = other.fieldType;
    this->localOffset = other.localOffset;
    this->localSize = other.localSize;
    this->integerValue = other.integerValue;
    this->realValue = other.realValue;
    this->textValue = other.textValue;
    this->partial     = other.partial;
}

FieldValue& FieldValue::operator=(FieldValue const& other) noexcept
{
    if(this != &other)
    {
        this->pageNumber   = other.pageNumber;
        this->fieldIndex   = other.fieldIndex;
        this->fieldName    = other.fieldName;
        this->fieldType    = other.fieldType;
        this->localOffset  = other.localOffset;
        this->localSize    = other.localSize;
        this->integerValue = other.integerValue;
        this->realValue    = other.realValue;
        this->textValue    = other.textValue;
        this->partial      = other.partial;
    }
    return *this;
}

uint32_t FieldValue::getPageNumber() const noexcept
{
    return this->pageNumber;
}

uint32_t FieldValue::getFieldIndex() const noexcept
{
    return this->fieldIndex;
}

string FieldValue::getFieldName() const noexcept
{
    return this->fieldName;
}

int64_t FieldValue::getFieldType() const noexcept
{
    return this->fieldType;
}

uint32_t FieldValue::getLocalOffset() const noexcept
{
    return this->localOffset;
}

uint32_t FieldValue::getLocalSize() const noexcept
{
    return this->localSize;
}

bool FieldValue::isNull() const noexcept
{
    return (this->fieldType == 0);
}

bool FieldValue::isInteger() const noexcept
{
    return SqliteFacility::isFieldTypeInteger(this->fieldType);
}

bool FieldValue::isReal() const noexcept
{
    return SqliteFacility::isFieldTypeReal(this->fieldType);
}

bool FieldValue::isText() const noexcept
{
    return SqliteFacility::isFieldTypeText(this->fieldType);
}

bool FieldValue::isBlob() const noexcept
{
    return SqliteFacility::isFieldTypeBlob(this->fieldType);
}

bool FieldValue::isPartial() const noexcept
{
    return this->partial;
}

int64_t FieldValue::getIntegerValue() const
{
    if(SqliteFacility::isFieldTypeInteger(this->fieldType))
    {
        return this->integerValue;
    }
    else
    {
        throw CommonException(CommonException::FAIL_GENERAL, "FieldValue: fieldType[" + to_string(this->fieldType) + "] not INTEGER!", __FILE__, __func__, __LINE__);
    }
}

double FieldValue::getRealValue() const
{
    if(SqliteFacility::isFieldTypeReal(this->fieldType))
    {
        return this->realValue;
    }
    else
    {
        throw CommonException(CommonException::FAIL_GENERAL, "FieldValue: fieldType[" + to_string(this->fieldType) + "] not REAL!", __FILE__, __func__, __LINE__);
    }
}

string FieldValue::getTextValue() const
{
    if(SqliteFacility::isFieldTypeText(this->fieldType))
    {
        return this->textValue;
    }
    else
    {
        throw CommonException(CommonException::FAIL_GENERAL, "FieldValue: fieldType[" + to_string(this->fieldType) + "] not TEXT!", __FILE__, __func__, __LINE__);
    }
}

pair<shared_ptr<char>, uint32_t> FieldValue::getBlobValue() const
{
    if(SqliteFacility::isFieldTypeBlob(this->fieldType))
    {
        return this->blobValue;
    }
    else
    {
        throw CommonException(CommonException::FAIL_GENERAL, "FieldValue: fieldType[" + to_string(this->fieldType) + "] not BLOB!", __FILE__, __func__, __LINE__);
    }
}

shared_ptr<FieldValue> FieldValue::parse(char* buff, uint32_t buffLength, uint32_t valueOffset, uint32_t pageNumber, uint32_t fieldIndex,
        string const& fieldName, int64_t fieldType, bool acceptPartialValue, bool& partial, uint32_t& valueBytes, Incident& incident) noexcept
{
    // ��飻
    if(valueOffset > buffLength)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "valueOffset[" + to_string(valueOffset) + "] overflow to buffLength[" + to_string(buffLength) + "]!", __FILE__, __func__, __LINE__);
        return shared_ptr<FieldValue>(nullptr);
    }

    incident.reset();

    // Null;
    if(fieldType == 0)
    {
        partial = false;
        valueBytes = 0;
        shared_ptr<FieldValue> fieldValue(new FieldValue(pageNumber, fieldIndex, fieldName));
        return fieldValue;
    }

    // Integer;
    if(SqliteFacility::isFieldTypeInteger(fieldType))
    {
        if(fieldType == 8)
        {
            partial = false;
            valueBytes = 0;
            shared_ptr<FieldValue> fieldValue(new FieldValue(pageNumber, fieldIndex, fieldName, fieldType, valueOffset, 0, (int64_t)0));
            return fieldValue;
        }
        else if(fieldType == 9)
        {
            partial = false;
            valueBytes = 0;
            shared_ptr<FieldValue> fieldValue(new FieldValue(pageNumber, fieldIndex, fieldName, fieldType, valueOffset, 0, (int64_t)1));
            return fieldValue;
        }
        else
        {
            int64_t integerValue = SqliteFacility::parseIntegerValue(buff + valueOffset, buffLength - valueOffset, fieldType, valueBytes, incident);
            if(!incident.succeeded()) return shared_ptr<FieldValue>(nullptr);

            partial = false;
            shared_ptr<FieldValue> fieldValue(new FieldValue(pageNumber, fieldIndex, fieldName, fieldType, valueOffset, valueBytes, integerValue));
            return fieldValue;
        }
    }

    // Real;
    if(SqliteFacility::isFieldTypeReal(fieldType))
    {
        partial = false;
        double realValue = SqliteFacility::parseRealValue(buff + valueOffset, buffLength - valueOffset, valueBytes, incident);
        if(!incident.succeeded()) return shared_ptr<FieldValue>(nullptr);

        shared_ptr<FieldValue> fieldValue(new FieldValue(pageNumber, fieldIndex, fieldName, fieldType, valueOffset, valueBytes, realValue));
        return fieldValue;
    }

    // Text;
    if(SqliteFacility::isFieldTypeText(fieldType))
    {
        bool partial;
        string textValue = SqliteFacility::parseTextValue(buff + valueOffset, buffLength - valueOffset, fieldType, acceptPartialValue, partial, valueBytes, incident);
        if(!incident.succeeded()) return shared_ptr<FieldValue>(nullptr);

        shared_ptr<FieldValue> fieldValue(new FieldValue(pageNumber, fieldIndex, fieldName, fieldType, valueOffset, valueBytes, textValue, partial));
        return fieldValue;
    }

    // Blob;
    if(SqliteFacility::isFieldTypeBlob(fieldType))
    {
        bool partial;
        shared_ptr<char> blobValue = SqliteFacility::parseBlobValue(buff + valueOffset, buffLength - valueOffset, fieldType, acceptPartialValue, partial, valueBytes, incident);
        if(!incident.succeeded()) return shared_ptr<FieldValue>(nullptr);

        shared_ptr<FieldValue> fieldValue(new FieldValue(pageNumber, fieldIndex, fieldName, fieldType, valueOffset, valueBytes, blobValue, partial));
        return fieldValue;
    }

    // Unkown;
    incident.set(SqliteReturnCode::FAIL_GENERAL, "Unknown FieldType[" + to_string(fieldType) + "]!", __FILE__, __func__, __LINE__);
    return shared_ptr<FieldValue>(nullptr);
}
