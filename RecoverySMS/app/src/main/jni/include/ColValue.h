#ifndef HUDUN_SQLITE_COLVALUE_H
#define HUDUN_SQLITE_COLVALUE_H

#include <iostream>
#include <memory>
#include <string>
#include <utility>
using namespace std;

namespace hudun
{
namespace sqlite
{
class ColValue
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
    StoreClass storeClass;
    int64_t integerValue;
    double realValue;
    string textValue;
    shared_ptr<char> blobBuff;
    uint32_t blobLength;

public:
    ColValue() noexcept;
    explicit ColValue(int64_t integerValue) noexcept;
    explicit ColValue(double realValue) noexcept;
    explicit ColValue(string const& textValue) noexcept;
    ColValue(shared_ptr<char> blobBuff, uint32_t blobLength);

    ColValue(ColValue const&) noexcept;
    ColValue& operator=(ColValue const&) noexcept;

    bool isNull() const noexcept;
    bool isInteger() const noexcept;
    bool isReal() const noexcept;
    bool isText() const noexcept;
    bool isBlob() const noexcept;

    int64_t getIntegerValue() const noexcept;
    double getRealValue() const noexcept;
    string getTextValue() const noexcept;
    pair<shared_ptr<char>, uint32_t> getBlobValue() const noexcept;

//    uint32_t getBlobLength() const;
//    string getBlobHexValue() const;
//    void copyBlob(char* buff) const;
};
ostream& operator<<(ostream&, hudun::sqlite::ColValue const&) noexcept;
}
}

#endif // HUDUN_SQLITE_COLVALUE_H
