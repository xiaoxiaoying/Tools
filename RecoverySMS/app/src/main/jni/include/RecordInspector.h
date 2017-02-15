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
 * ��¼�������
 * ������ݿ�ĳ��λ���Ƿ����Ϊĳ���������ݿ��ļ�¼�������ռ�¼��ģʽ��ȡ��¼���ܵ�ֵ��
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
     * ���ҳ��ָ��ƫ��λ�ÿ�ʼ�������Ƿ���ϼ�¼�����ֶ����Ͷ����������
     * @param page ҳ�����ݣ�
     * @param offset ҳ��ƫ�ƣ�
     * @param varints ��ƫ��λ�ÿ�ʼ�������ġ��ɳ�Ա����inspectVarintNumָ�������ı䳤�������������ֵΪ�棬����Щ�䳤���������¼ͷ��
     * @param varintsBytes �������Ķ���䳤����varintsռ�õ��ֽ�������
     * @param incident �¹ʣ�
     * @return �Ƿ���ϼ�¼�ֶ�����������
     * @note ���̰߳�ȫ��
     */
    bool inspect(char* page, uint32_t offset, vector<int64_t>& varints, uint32_t& varintsBytes, hudun::common::Incident& incident) const noexcept;

    /**
     * ��ҳ��ָ��ƫ��λ�ã����ݼ�¼ͷ����ȡ��¼ֵ��
     * @param pageNumber ����ҳ���ţ�
     * @param page ҳ�����ݣ�
     * @param varints ��¼ͷ��
     * @param varintsOffset ��¼ͷƫ��λ�ã�
     * @param varintsBytes ��¼ͷvarintsռ�õ��ֽ�������
     * @param record (OUT)���ؼ�¼ֵ��
     * @param recordValueBytes (OUT)���ؼ�¼ֵռ�õ��ֽ�������
     * @param incident �¹ʣ�
     * @note ���̰߳�ȫ��
     */
    void inspectFieldValues(uint32_t pageNumber, char* page, vector<int64_t> const& varints, uint32_t varintsOffset, uint32_t const& varintsBytes,
                                       shared_ptr<Record>& record, uint32_t& recordValueBytes, hudun::common::Incident& incident) const noexcept;

    /**
     * �������ݿ��Ķ�����䣨DDL���Ƶ������ݿ���¼��Ӧ�ļ�¼�������
     * @param table ���ݿ��
     * @param pageLength ���ݿ��ҳ���ȣ����ڿ����ڼ���¼�ֶ����ͻ��߼�¼�ֶ�ֵʱ������Խ�磻
     * @return �Ƶ����ļ�¼�������
     */
    static shared_ptr<RecordInspector> deduce(Database& database, hudun::sqlite::schema::Table const& table, bool processOverflowRecord) noexcept;
};
ostream& operator<<(ostream&, RecordInspector const&) noexcept;
}
}
#endif // HUDUN_SQLITE_RECORDINSPECTOR_H
