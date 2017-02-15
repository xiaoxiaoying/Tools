#include <string>
#include <iostream>
using namespace std;

#include "../include/FieldTypeInspector.h"
using namespace hudun::sqlite;

#include "../include/Field.h"
#include "../include/Strto.h"

using namespace hudun::sqlite::schema;

FieldTypeInspector::FieldTypeInspector(Field::TypeAffinity typeAffinity) noexcept
{
    this->typeAffinity = typeAffinity;
}

FieldTypeInspector::FieldTypeInspector(FieldTypeInspector const& other) noexcept
{
    this->typeAffinity = other.typeAffinity;
    this->fieldTypeFlags = other.fieldTypeFlags;
    this->minValue = other.minValue;
    this->maxValue = other.maxValue;
}

FieldTypeInspector& FieldTypeInspector::operator=(FieldTypeInspector const& other) noexcept
{
    if(this != &other)
    {
        this->typeAffinity = other.typeAffinity;
        this->fieldTypeFlags = other.fieldTypeFlags;
        this->minValue = other.minValue;
        this->maxValue = other.maxValue;
    }
    return *this;
}

bool FieldTypeInspector::isIntegerPrimaryKey() const noexcept
{
    // INTEGER;
    if(this->typeAffinity != Field::TypeAffinity::SQLITE_INTEGER) return false;

    // NULLABLE, MIN, MAX;
    uint32_t nmm = FieldTypeFlag::NULLABLE | FieldTypeFlag::MIN | FieldTypeFlag::MAX;
    if((this->fieldTypeFlags & nmm) != nmm) return false;

    return ((this->minValue == 0) & (this->maxValue == 0));
}

void FieldTypeInspector::setNullable() noexcept
{
    this->fieldTypeFlags |= FieldTypeFlag::NULLABLE;
}

void FieldTypeInspector::setMinValue(int64_t minValue) noexcept
{
    this->fieldTypeFlags |= FieldTypeFlag::MIN;
    this->minValue = minValue;
}

void FieldTypeInspector::setMaxValue(int64_t maxValue) noexcept
{
    this->fieldTypeFlags |= FieldTypeFlag::MAX;
    this->maxValue = maxValue;
}

bool FieldTypeInspector::inspect(int64_t fieldType) const noexcept
{
    // FieldTypeFlag: NULLABLE;
    if(((this->fieldTypeFlags & FieldTypeFlag::NULLABLE) == FieldTypeFlag::NULLABLE) && (fieldType == 0)) return true;

    // check typeAffinity;
    switch(this->typeAffinity)
    {
    case Field::TypeAffinity::SQLITE_INTEGER:
        if((fieldType <= 0) || (fieldType > 9) || (fieldType == 7)) return false;
        break;
    case Field::TypeAffinity::SQLITE_REAL:
        if(fieldType != 7) return false;
        break;

    case Field::TypeAffinity::SQLITE_TEXT:
        if((fieldType < 13) || ((fieldType & 1) == 0)) return false;
        break;
    case Field::TypeAffinity::SQLITE_BLOB:
        if((fieldType < 12) || ((fieldType & 1) == 1)) return false;
        break;
    case Field::TypeAffinity::SQLITE_NUMERIC:
        if((fieldType <= 0) || (fieldType> 9)) return false;
        break;
    default:
        return false;
    }

    // FieldTypeFlag: MIN;
    if(((this->fieldTypeFlags & FieldTypeFlag::MIN) == FieldTypeFlag::MIN) && (fieldType < this->minValue)) return false;

    // FieldTypeFlag: MAX;
    if(((this->fieldTypeFlags & FieldTypeFlag::MAX) == FieldTypeFlag::MAX) && (fieldType > this->maxValue)) return false;

    return true;
}

ostream& hudun::sqlite::operator<<(ostream& out, FieldTypeInspector const& fieldTypeInspector) noexcept
{
    // StoreClass;
    switch(fieldTypeInspector.typeAffinity)
    {
    case Field::TypeAffinity::SQLITE_INTEGER:
        out << "SQLITE_INTEGER";
        break;
    case Field::TypeAffinity::SQLITE_REAL:
        out << "SQLITE_REAL";
        break;
    case Field::TypeAffinity::SQLITE_TEXT:
        out << "SQLITE_TEXT";
        break;
    case Field::TypeAffinity::SQLITE_BLOB:
        out << "SQLITE_BLOB";
        break;
    case Field::TypeAffinity::SQLITE_NUMERIC:
        out << "SQLITE_NUMERIC";
        break;
    default:
        out << "Unknown TypeAffinity: " + to_string(fieldTypeInspector.typeAffinity);
    }

    // fieldTypeFlags
    if(fieldTypeInspector.fieldTypeFlags == FieldTypeInspector::FieldTypeFlag::NONE)
    {
        out << "\tFlag: NONE";
    }
    else
    {
        // NULLABLE;
        if((fieldTypeInspector.fieldTypeFlags & FieldTypeInspector::FieldTypeFlag::NULLABLE) == FieldTypeInspector::FieldTypeFlag::NULLABLE)
        {
            out << "\tNULLABLE";
        }

        // MIN
        if((fieldTypeInspector.fieldTypeFlags & FieldTypeInspector::FieldTypeFlag::MIN) == FieldTypeInspector::FieldTypeFlag::MIN)
        {
            out << "\tMIN[" << fieldTypeInspector.minValue << "]";
        }

        // MAX
        if((fieldTypeInspector.fieldTypeFlags & FieldTypeInspector::FieldTypeFlag::MAX) == FieldTypeInspector::FieldTypeFlag::MAX)
        {
            out << "\tMAX[" << fieldTypeInspector.maxValue << "]";
        }
    }

    return out;
}
