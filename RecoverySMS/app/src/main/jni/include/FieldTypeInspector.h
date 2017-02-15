#ifndef HUDUN_SQLITE_FIELDTYPEINSPECTOR_H
#define HUDUN_SQLITE_FIELDTYPEINSPECTOR_H
#include <string>
using namespace std;

#include "Field.h"

namespace hudun
{
namespace sqlite
{
/**
 * 字段类型检查器；
 * 数据库表中定义的各个字段的类型以及约束具有一定特征，本类用于识别当前位置的字段类型是否符合这些特征；
 *
 * @author Wu Deyou
 * @date 2015-12-16
 */
class FieldTypeInspector
{
public:
    enum FieldTypeFlag: uint32_t
    {
        NONE                = 0,
        NULLABLE            = 0x0001,
        MIN                 = 0x0002,
        MAX                 = 0x0004
    };
private:
    friend ostream& operator<<(ostream& out, FieldTypeInspector const& fieldTypeInspector) noexcept;
    hudun::sqlite::schema::Field::TypeAffinity typeAffinity;
    uint32_t fieldTypeFlags = FieldTypeFlag::NONE;
    int64_t minValue = 0;
    int64_t maxValue = 0;

public:
    FieldTypeInspector(hudun::sqlite::schema::Field::TypeAffinity typeAffinity) noexcept;
    FieldTypeInspector(FieldTypeInspector const&) noexcept;
    FieldTypeInspector& operator=(FieldTypeInspector const&) noexcept;

    bool isIntegerPrimaryKey() const noexcept;

    void setNullable() noexcept;
    void setMinValue(int64_t minValue) noexcept;
    void setMaxValue(int64_t maxValue) noexcept;

    bool inspect(int64_t fieldType) const noexcept;

};
ostream& operator<<(ostream& out, FieldTypeInspector const& fieldTypeInspector) noexcept;
}
}
#endif // HUDUN_SQLITE_FIELDTYPEINSPECTOR_H


