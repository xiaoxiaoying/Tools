#ifndef HUDUN_SQLITE_FIELDVALUE_H
#define HUDUN_SQLITE_FIELDVALUE_H
#include <string>
#include <memory>
using namespace std;

#include "Incident.h"

namespace hudun
{
namespace sqlite
{
class FieldValue
{
public:
    enum StoreClass: uint8_t
    {
        SQLITE_UNKNOWN  = 0,
        SQLITE_INTEGER  = 0x01,
        SQLITE_REAL     = 0x02,
        SQLITE_TEXT     = 0x03,
        SQLITE_BLOB     = 0x04,
        SQLITE_NULL     = 0x05
    };

protected:
    uint32_t pageNumber;
    uint32_t fieldIndex;
    string fieldName;
    int64_t fieldType;
    uint32_t localOffset;
    uint32_t localSize;
    bool partial;

    int64_t integerValue;
    double realValue;
    string textValue;
    pair<shared_ptr<char>, uint32_t> blobValue;

public:
    FieldValue(uint32_t pageNumber, uint32_t fieldIndex, string const& fieldName);
    FieldValue(uint32_t pageNumber, uint32_t fieldIndex, string const& fieldName, int64_t fieldType, uint32_t localOffset, uint32_t localSize, int64_t integerValue) noexcept;
    FieldValue(uint32_t pageNumber, uint32_t fieldIndex, string const& fieldName, int64_t fieldType, uint32_t localOffset, uint32_t localSize, double realValue) noexcept;
    FieldValue(uint32_t pageNumber, uint32_t fieldIndex, string const& fieldName, int64_t fieldType, uint32_t localOffset, uint32_t localSize, string const& textValue, bool partial) noexcept;
    FieldValue(uint32_t pageNumber, uint32_t fieldIndex, string const& fieldName, int64_t fieldType, uint32_t localOffset, uint32_t localSize, shared_ptr<char> const& blobValue, bool partial) noexcept;

    FieldValue(FieldValue const&) noexcept;
    FieldValue& operator=(FieldValue const&) noexcept;

    uint32_t getPageNumber() const noexcept;
    uint32_t getFieldIndex() const noexcept;
    string getFieldName() const noexcept;
    int64_t getFieldType() const noexcept;
    uint32_t getLocalOffset() const noexcept;
    uint32_t getLocalSize() const noexcept;

    bool isNull() const noexcept;
    bool isInteger() const noexcept;
    bool isReal() const noexcept;
    bool isText() const noexcept;
    bool isBlob() const noexcept;
    bool isPartial() const noexcept;

    int64_t getIntegerValue() const;
    double getRealValue() const;
    string getTextValue() const;
    pair<shared_ptr<char>, uint32_t> getBlobValue() const;

    /**
     * 从页面page偏移valueOffset处，解析域值。
     * @param pageNumber 页面编号；
     * @param buff 数据；
     * @param valueOffset 域值偏移；
     * @param buffLength 页面长度；
     * @param fieldIndex 域索引；
     * @param fieldName 域名称；
     * @param fieldType 域类型；
     * @param valueBytes (OUT)域值字节长度；
     * @param incident (OUT)事故；
     * @return 域值。
     */
    static shared_ptr<FieldValue> parse(char* buff, uint32_t buffLength, uint32_t valueOffset, uint32_t pageNumber, uint32_t fieldIndex,
        string const& fieldName, int64_t fieldType, bool acceptPartialValue, bool& partial, uint32_t& valueBytes, hudun::common::Incident& incident) noexcept;
};
}
}

#endif // HUDUN_SQLITE_FIELDVALUE_H
