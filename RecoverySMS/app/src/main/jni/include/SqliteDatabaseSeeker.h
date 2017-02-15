#ifndef HUDUN_SQLITE_SQLITEDATABASESEEKER_H
#define HUDUN_SQLITE_SQLITEDATABASESEEKER_H

#include "Database.h"
#include "Incident.h"
#include "RecordFilter.h"
#include "RecordInspector.h"
#include "SeekEventHandler.h"
#include "SeekProgressIndicator.h"
#include "Table.h"

namespace hudun
{
namespace sqlite
{
/**
 * Sqlite���ݿ���������
 * ��Sqlite���ݿ⣨�ļ����У�����ָ����ļ�¼��
 *
 * @author Wu Deyou
 * @date 2016-05-17
 */
class SqliteDatabaseSeeker
{
private:
    Database* database;
    RecordFilter* recordFilter;
    bool processOverflowRecord;
    shared_ptr<RecordInspector> recordInspector;

public:
    /**
     * ���캯����
     * @param database Sqlite���ݿ����ָ�룻
     * @param table ����Ŀ�����ݿ�����ָ�룻
     * @param recordFilter ��¼����������ָ�롣
     * @param processOverflowRecord �Ƿ��������¼��
     */
    SqliteDatabaseSeeker(Database& database, hudun::sqlite::schema::Table const& table, RecordFilter* recordFilter, bool processOverflowRecord) noexcept;

    /**
     * ���ƹ��캯����
     */
    SqliteDatabaseSeeker(SqliteDatabaseSeeker const&) noexcept;

    /**
     * ����ֵ������
     */
    SqliteDatabaseSeeker& operator=(SqliteDatabaseSeeker const&) noexcept;

    /**
     * ����������
     */
    ~SqliteDatabaseSeeker();

     /**
     * ������¼���������������¼��������汾����
     * @param threadNum �����߳�����
     * @param seekProgressIndicator ����ָʾ�����ɿգ���
     * @param seekEventHandler �����¼�������������Ϊ�գ�������̰߳�ȫ��
     * @param incident �¹ʣ�
     * @return �������ļ�¼������
     */
    uint32_t seek(uint32_t threadNum, SeekProgressIndicator* seekProgressIndicator, SeekEventHandler* seekEventHandler, hudun::common::Incident& incident) noexcept;

     /**
     * ������¼�������ǹ��������¼��������汾����
     * @param threadNum �����߳�����
     * @param seekProgressIndicator ����ָʾ�����ɿգ���
     * @param seekEventHandler �����¼����������飬ÿ���̵߳���ʹ��һ��������������
     * @param incident �¹ʣ�
     * @return �������ļ�¼������
     */
    uint32_t seek(uint32_t threadNum, SeekProgressIndicator* seekProgressIndicator, SeekEventHandler** seekEventHandlers, hudun::common::Incident& incident) noexcept;
};
}
}

#endif // HUDUN_SQLITE_SQLITEDATABASESEEKER_H


