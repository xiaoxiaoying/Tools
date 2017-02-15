#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
using namespace std;

#include "../include/StringFacility.h"
using namespace hudun::common;

#include "../include/FieldValue.h"
#include "../include/Record.h"
#include "../include/RecordSkippedHead.h"
#include "../include/Strto.h"

using namespace hudun::sqlite;

Record::Record() noexcept
{
    this->pageNumber = 0;
    this->recordSkippedHead = nullptr;
}

Record::Record(Record const& other) noexcept
{
    this->pageNumber = other.pageNumber;
    this->recordSkippedHead = other.recordSkippedHead;
    this->fieldIndexToFieldValueMap = other.fieldIndexToFieldValueMap;
    this->fieldNameToFieldValueMap = other.fieldNameToFieldValueMap;
}

Record& Record::operator=(Record const& other) noexcept
{
    if(this != &other)
    {
        this->pageNumber = other.pageNumber;
        this->recordSkippedHead = other.recordSkippedHead;
        this->fieldIndexToFieldValueMap = other.fieldIndexToFieldValueMap;
        this->fieldNameToFieldValueMap = other.fieldNameToFieldValueMap;
    }
    return *this;
}

void Record::add(shared_ptr<FieldValue> const& fieldValue) noexcept
{
    this->fieldIndexToFieldValueMap.emplace(fieldValue->getFieldIndex(), fieldValue);
    string upperFieldName = fieldValue->getFieldName();
    StringFacility::toupper(upperFieldName);
    this->fieldNameToFieldValueMap.emplace(upperFieldName, fieldValue);
}

void Record::clear() noexcept
{
    this->pageNumber = 0;
    this->recordSkippedHead = nullptr;
    this->fieldIndexToFieldValueMap.clear();
    this->fieldNameToFieldValueMap.clear();
}

uint32_t Record::getFieldValueNum() const noexcept
{
    return (uint32_t)this->fieldIndexToFieldValueMap.size();
}

vector<uint32_t> Record::getAllFieldIndexes() const noexcept
{
    vector<uint32_t> fieldIndexes;
    for(map<uint32_t, shared_ptr<FieldValue>>::const_iterator it = this->fieldIndexToFieldValueMap.begin(); it != this->fieldIndexToFieldValueMap.end(); ++ it)
    {
        fieldIndexes.push_back(it->first);
    }
    return fieldIndexes;
}

vector<string> Record::getAllFieldNames() const noexcept
{
    vector<string> fieldNames;
    for(map<uint32_t, shared_ptr<FieldValue>>::const_iterator it = this->fieldIndexToFieldValueMap.begin(); it != this->fieldIndexToFieldValueMap.end(); ++ it)
    {

        fieldNames.push_back(it->second->getFieldName());
    }
    return fieldNames;
}

const shared_ptr<FieldValue> Record::getFieldValueByFieldIndex(uint32_t fieldIndex) const noexcept
{
    map<uint32_t, shared_ptr<FieldValue>>::const_iterator it = this->fieldIndexToFieldValueMap.find (fieldIndex);
    if(it == this->fieldIndexToFieldValueMap.end())
    {
        return shared_ptr<FieldValue>(nullptr);
    }
    else
    {
        return it->second;
    }
}

const shared_ptr<FieldValue> Record::getFieldValueByFieldName(string const& fieldName) const noexcept
{
    string upperFieldName = fieldName;
    StringFacility::toupper(upperFieldName);
    map<string, shared_ptr<FieldValue>>::const_iterator it = this->fieldNameToFieldValueMap.find (upperFieldName);
    if(it == this->fieldNameToFieldValueMap.end())
    {
        return shared_ptr<FieldValue>(nullptr);
    }
    else
    {
        return it->second;
    }
}

void Record::setPageNumber(uint32_t pageNumber) noexcept
{
    this->pageNumber = pageNumber;
}

uint32_t Record::getPageNumber() const noexcept
{
    return this->pageNumber;
}

void Record::setRecordSkippedHead(shared_ptr<RecordSkippedHead> const& recordSkippedHead) noexcept
{
    this->recordSkippedHead = recordSkippedHead;
}

shared_ptr<RecordSkippedHead> Record::getRecordSkippedHead() const noexcept
{
    return this->recordSkippedHead;
}

ostream& hudun::sqlite::operator<<(ostream& out, Record const& record) noexcept
{
    const vector<uint32_t> fieldIndexes = record.getAllFieldIndexes();

    out << "Page Number: " << record.getPageNumber() << endl;

    for(uint32_t i = 0; i < fieldIndexes.size(); i ++)
    {
        shared_ptr<FieldValue> fieldValue = record.getFieldValueByFieldIndex(fieldIndexes[i]);
        out << fieldValue->getFieldName() << ": ";
        if(fieldValue->isNull())
        {
            out << "NULL" << endl;
        }
        else if(fieldValue->isInteger())
        {
            out << fieldValue->getIntegerValue() << endl;
        }
        else if(fieldValue->isReal())
        {
            out << fieldValue->getRealValue() << endl;
        }
        else if(fieldValue->isText())
        {
            string textValue = fieldValue->getTextValue();
            out << StringFacility::washUtf8(textValue) << endl;
        }
        else if(fieldValue->isBlob())
        {
            out << "NOT SUPPORTED!" << endl;
        }
        else
        {
            out << "UNKNOWN fieldType[" + to_string(fieldValue->getFieldType()) + "]!" << endl;
        }
    }
    return out;
}
