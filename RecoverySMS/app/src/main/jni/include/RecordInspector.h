#ifndef HUDUN_SQLITE_RECORDINSPECTOR_H
#define HUDUN_SQLITE_RECORDINSPECTOR_H
#include <string>
#include <memory>
#include <vector>
using namespace std;

#include "Database.h"
#include "FieldTypeInspector.h"
#include "FieldValue.h"
#include "Incident.h"
#include "Record.h"
#include "Table.h"

namespace hudun
{
namespace sqlite
{
/**
 * 记录检查器；
 * 检查数据库某个位置是否可能为某种类型数据库表的记录，并按照记录的模式提取记录可能的值；
 *
 * @author Wu Deyou
 * @date 2016-05-17
 */
class RecordInspector
{
public:

private:
    friend ostream& operator<<(ostream&, RecordInspector const&) noexcept;

    Database* database;
    const hudun::sqlite::schema::Table* table;
    bool processOverflowRecord;

    vector<FieldTypeInspector> fieldTypeInspectors;
    int integerPrimaryKeyFieldIndex;

    uint32_t totalPages;
    uint32_t pageSize;
    uint32_t pageLength;
    uint8_t minEmbeddedFraction;
    uint32_t minLocalSize;
    uint32_t maxLocalSize;

    void pushBack(FieldTypeInspector const&) noexcept;
    void setIntegerPrimaryKeyFieldIndex(int integerPrimaryKeyFieldIndex) noexcept;
    int speculateFirstFieldTypeAndValue(uint32_t pageNumber, char* page, const RecordSkippedHead* recordSkippedHead, uint32_t dataOffset,
        uint32_t& firstTypeBytes, int64_t& firstType, uint32_t& firstValueBytes, shared_ptr<FieldValue>& firstValue, hudun::common::Incident& incident) const noexcept;

    RecordInspector(Database& database, hudun::sqlite::schema::Table const& table, bool processOverflowRecord) noexcept;

public:
    RecordInspector(RecordInspector const&) noexcept;
    RecordInspector& operator=(RecordInspector const&) noexcept;

    /**
     * 检查页面指定偏移位置开始的数据是否符合记录各个字段类型定义的特征；
     * @param page 页面数据；
     * @param offset 页面偏移；
     * @param varints 从偏移位置开始解析出的、由成员变量inspectVarintNum指定个数的变长整数。如果返回值为真，则这些变长整数代表记录头；
     * @param varintsBytes 解析出的多个变长整数varints占用的字节数量；
     * @param incident 事故；
     * @return 是否符合记录字段类型特征；
     * @note 多线程安全；
     */
    bool inspect(char* page, uint32_t offset, vector<int64_t>& varints, uint32_t& varintsBytes, hudun::common::Incident& incident) const noexcept;

    /**
     * 从页面指定偏移位置，根据记录头，提取记录值；
     * @param pageNumber 所在页面编号；
     * @param page 页面数据；
     * @param varints 记录头；
     * @param varintsOffset 记录头偏移位置；
     * @param varintsBytes 记录头varints占用的字节数量；
     * @param record (OUT)返回记录值；
     * @param recordValueBytes (OUT)返回记录值占用的字节数量；
     * @param incident 事故；
     * @note 多线程安全；
     */
    void inspectFieldValues(uint32_t pageNumber, char* page, vector<int64_t> const& varints, uint32_t varintsOffset, uint32_t const& varintsBytes,
                                       shared_ptr<Record>& record, uint32_t& recordValueBytes, hudun::common::Incident& incident) const noexcept;

    /**
     * 根据数据库表的定义语句（DDL）推导该数据库表记录对应的记录检查器；
     * @param table 数据库表；
     * @param pageLength 数据库的页长度，用于控制在检查记录字段类型或者记录字段值时不至于越界；
     * @return 推导出的记录检查器；
     */
    static shared_ptr<RecordInspector> deduce(Database& database, hudun::sqlite::schema::Table const& table, bool processOverflowRecord) noexcept;
};
ostream& operator<<(ostream&, RecordInspector const&) noexcept;
}
}
#endif // HUDUN_SQLITE_RECORDINSPECTOR_H
