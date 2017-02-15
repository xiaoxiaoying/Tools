#ifndef HUDUN_SQLITE_SEEKEVENTHANDLER_H
#define HUDUN_SQLITE_SEEKEVENTHANDLER_H
#include <memory>
using namespace std;

#include "Record.h"

namespace hudun
{
namespace sqlite
{
/**
 * �����¼���������
 * ���ඨ����һ���ӿڣ�����������Sqlite���ݿ������, ���崦�����������ϵ����ݿ��¼��
 *
 * @author Wu Deyou
 * @date 2015-12-21
 */
class SeekEventHandler
{
public:

    /**
     * ��������������
     */
    virtual ~SeekEventHandler() = default;

    /**
     * ������Sqliteҳ��֮ǰ���ã�
     * @param pageNumber ҳ���š�
     */
    virtual void beforeSeekPage(uint32_t pageNumber) noexcept = 0;

    /**
     * ������Sqliteҳ��֮����ã�
     * @param pageNumber ҳ���š�
     */
    virtual void afterSeekPage(uint32_t pageNumber) noexcept = 0;

    /**
     * ���������ϵļ�¼����ã�
     * @param record ��¼��
     * @return �ҵ��ļ�¼������0��1����
     */
    virtual int seekedRecord(shared_ptr<Record>& record) noexcept = 0;
};
}
}
#endif // HUDUN_SQLITE_SEEKEVENTHANDLER_H
