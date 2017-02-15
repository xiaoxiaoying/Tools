#ifndef HUDUN_SQLITE_SCHEMA_FIELD_H
#define HUDUN_SQLITE_SCHEMA_FIELD_H
#include <string>
#include <memory>
using namespace std;

namespace hudun
{
namespace sqlite
{
namespace schema
{
class Field
{
public:
    enum TypeAffinity: uint8_t
    {
        SQLITE_UNKNOWN  = 0,
        SQLITE_INTEGER  = 0x01,
        SQLITE_REAL     = 0x02,
        SQLITE_TEXT     = 0x03,
        SQLITE_BLOB     = 0x04,
        SQLITE_NUMERIC  = 0x06
    };

    enum Flag: uint32_t
    {
        NONE                = 0,
        PRIMARY_KEY         = 0x0001,
        FOREIGN_KEY         = 0x0002,
        NOT_NULL            = 0x0004,
        DEFAULT_VALUE       = 0x0008,
        UNIQUE              = 0x0010,
        REFERENCES          = 0x0020,
        AUTOINCREMENT       = 0x0040,

        TYPE_LENGTH         = 0x0100,
        PRECISION_LENGTH    = 0x0200

    };

private:
    uint32_t fieldIndex;
    string fieldName;
    string declaredType;
    TypeAffinity typeAffinity = TypeAffinity::SQLITE_UNKNOWN;
    uint32_t flags = Flag::NONE;

    string defaultValue;

    uint32_t typeLength;
    uint32_t precisionLength;

    string referenceTable;
    string referenceField;

public:
    Field(uint32_t fieldIndex, string const& fieldName, string const& declaredType) noexcept;
    Field(uint32_t fieldIndex, string const& fieldName, string const& declaredType, uint32_t typeLength) noexcept;
    Field(uint32_t fieldIndex, string const& fieldName, string const& declaredType, uint32_t typeLength, uint32_t precisionLength) noexcept;
    Field(Field const&) noexcept;
    Field& operator=(Field const&) noexcept;
    ~Field();

    void setPrimaryKey() noexcept;
    void setForeignKey() noexcept;
    void setNotNull() noexcept;
    void setDefaultValue(string const& defaultValue) noexcept;
    void setUnique() noexcept;
    void setReferences(string referenceTable, string referenceField) noexcept;
    void setAutoIncrment() noexcept;

    bool check() const noexcept;

    bool isINTEGER() const noexcept;
    bool isREAL() const noexcept;
    bool isTEXT() const noexcept;
    bool isBLOB() const noexcept;
    bool isNUMERIC() const noexcept;

    bool isPrimaryKey() const noexcept;
    bool isForeignKey() const noexcept;
    bool isNotNull() const noexcept;
    bool hasDefaultValue() const noexcept;
    bool isUnique() const noexcept;
    bool isReferences() const noexcept;
    bool isAutoIncrement() const noexcept;
    bool hasTypeLength() const noexcept;
    bool hasPrecisionLength() const noexcept;

    uint32_t getFieldIndex() const noexcept;
    string getFieldName() const noexcept;
    string getDeclaredType() const noexcept;
    TypeAffinity getTypeAffinity() const noexcept;

    string getDefaultValue() const;

    uint32_t getTypeLength() const;
    uint32_t getPrecisionLength() const;

    string getReferenceTable() const;
    string getReferenceField() const;

    string serialize(string prefix) const;

    static shared_ptr<Field> parse(string& rest, uint32_t fieldIndex);
};
}
}
}
#endif // HUDUN_SQLITE_SCHEMA_FIELD_H
