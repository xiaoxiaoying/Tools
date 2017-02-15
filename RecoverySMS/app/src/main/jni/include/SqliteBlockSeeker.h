#ifndef HUDUN_SQLITE_SQLITEBLOCKSEEKER_H
#define HUDUN_SQLITE_SQLITEBLOCKSEEKER_H

#include <memory>
using namespace std;

#include "Incident.h"
#include "RecordFilter.h"
#include "RecordInspector.h"
#include "SeekEventHandler.h"
#include "SeekProgressIndicator.h"

namespace hudun
{
namespace sqlite
{
/**
 * Sqlite���ݿ����������
 * ��ָ�������ݿ��У�����Sqlite���ݿ�ָ����ļ�¼��
 *
 * @author Wu Deyou
 * @date 2016-05-17
 */
class SqliteBlockSeeker
{
private:
    uint32_t pageSize;
    uint32_t pageLength;
    RecordFilter* recordFilter;
    RecordInspector* recordInspector;
    uint32_t pagesPerProgress = 100;

public:
    /**
     * ���캯����
     * @param table ����Ŀ�����ݿ�����
     * @param pageLength Qqlite���ݿ�ҳ�泤�ȣ�
     * @param recordFilter ��¼����������ָ�롣
     */
    SqliteBlockSeeker(uint32_t pageSize, uint32_t pageLength, RecordInspector* recordInspector, RecordFilter* recordFilter) noexcept;

    /**
     * ���ƹ��캯����
     */
    SqliteBlockSeeker(SqliteBlockSeeker const&) noexcept;

    /**
     * ����ֵ������
     */
    SqliteBlockSeeker& operator=(SqliteBlockSeeker const&) noexcept;

    /**
     * ����������
     */
    ~SqliteBlockSeeker();

     /**
     * ������¼������
     * @param pageBlock ���ݿ飻
     * @param pageNumberStart ���ݿ鿪ʼҳ���ţ�
     * @param pageNum ���ݿ������ҳ������
     * @param seekEventHandler �����¼�����������ָ�룻
     * @return �������ļ�¼������
     * @note ����ⲿע���recordFilter��seekEventHandler���̰߳�ȫ���򱾷���Ҳ���̰߳�ȫ�ġ�
     */
    uint32_t seek(char* pageBlock, uint32_t pageNumberStart, uint32_t pageNum, SeekProgressIndicator* seekProgressIndicator, SeekEventHandler* seekEventHandler) noexcept;
};
}
}

#endif // HUDUN_SQLITE_SQLITEBLOCKSEEKER_H

