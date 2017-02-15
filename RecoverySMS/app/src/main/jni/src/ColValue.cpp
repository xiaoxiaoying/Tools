#include <string>
#include <iostream>
using namespace std;

#include "../include/CommonFacility.h"
#include "../include/CommonException.h"
using namespace hudun::common;

#include "../include/ColValue.h"
using namespace hudun::sqlite;

ColValue::ColValue() noexcept
{
    this->storeClass = StoreClass::SQLITE_NULL;
}

ColValue::ColValue(int64_t integerValue) noexcept
{
    this->storeClass = StoreClass::SQLITE_INTEGER;
    this->integerValue = integerValue;
}

ColValue::ColValue(double realValue) noexcept
{
    this->storeClass = StoreClass::SQLITE_REAL;
    this->realValue = realValue;
}

ColValue::ColValue(string const& textValue) noexcept
{
    this->storeClass = StoreClass::SQLITE_TEXT;
    this->textValue = textValue;
}

ColValue::ColValue(shared_ptr<char> blobBuff, uint32_t blobLength)
{
    this->storeClass = StoreClass::SQLITE_BLOB;
    this->blobBuff = blobBuff;
    this->blobLength = blobLength;
}

ColValue::ColValue(ColValue const& other) noexcept
{
    this->storeClass = other.storeClass;
    this->integerValue = other.integerValue;
    this->realValue = other.realValue;
    this->textValue = other.textValue;
    this->blobBuff = other.blobBuff;
    this->blobLength = other.blobLength;
}

ColValue& ColValue::operator=(ColValue const& other) noexcept
{
    if(this != &other)
    {
        this->storeClass = other.storeClass;
        this->integerValue = other.integerValue;
        this->realValue = other.realValue;
        this->textValue = other.textValue;
        this->blobBuff = other.blobBuff;
        this->blobLength = other.blobLength;
    }
    return *this;
}

bool ColValue::isNull() const noexcept
{
    return (this->storeClass == StoreClass::SQLITE_NULL);
}

bool ColValue::isInteger() const noexcept
{
    return (this->storeClass == StoreClass::SQLITE_INTEGER);
}

bool ColValue::isReal() const noexcept
{
    return (this->storeClass == StoreClass::SQLITE_REAL);
}

bool ColValue::isText() const noexcept
{
    return (this->storeClass == StoreClass::SQLITE_TEXT);
}

bool ColValue::isBlob() const noexcept
{
    return (this->storeClass == StoreClass::SQLITE_BLOB);
}

int64_t ColValue::getIntegerValue() const noexcept
{
    return this->integerValue;
}

double ColValue::getRealValue() const noexcept
{
    return this->realValue;
}

string ColValue::getTextValue() const noexcept
{
    return this->textValue;
}

pair<shared_ptr<char>, uint32_t> ColValue::getBlobValue() const noexcept
{
    return pair<shared_ptr<char>, uint32_t>(this->blobBuff, this->blobLength);
}

ostream& hudun::sqlite::operator<<(ostream& out, ColValue const& colValue) noexcept
{
    if(colValue.isInteger())
    {
        out << colValue.getIntegerValue();
    }
    else if(colValue.isReal())
    {
        out << colValue.getRealValue();
    }
    else if(colValue.isText())
    {
        out << colValue.getTextValue();
    }
    else if(colValue.isBlob())
    {
        pair<shared_ptr<char>, uint32_t> blobValue = colValue.getBlobValue();
        string hexString = CommonFacility::toHexString(blobValue.first.get(), blobValue.second);
        out << hexString;
    }
    else if(colValue.isNull())
    {
        out << "<NULL>";
    }
    else
    {
        out << "!_!-!ERROR: unknown storeClass!-!_!";
    }
    return out;
}
