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
 * 搜索事件处理器；
 * 本类定义了一个接口，用于在搜索Sqlite数据块过程中, 具体处理搜索到符合的数据库记录。
 *
 * @author Wu Deyou
 * @date 2015-12-21
 */
class SeekEventHandler
{
public:

    /**
     * 纯虚析构函数；
     */
    virtual ~SeekEventHandler() = default;

    /**
     * 在搜索Sqlite页面之前调用；
     * @param pageNumber 页面编号。
     */
    virtual void beforeSeekPage(uint32_t pageNumber) noexcept = 0;

    /**
     * 在搜索Sqlite页面之后调用；
     * @param pageNumber 页面编号。
     */
    virtual void afterSeekPage(uint32_t pageNumber) noexcept = 0;

    /**
     * 在搜索符合的记录后调用；
     * @param record 记录；
     * @return 找到的记录数量（0或1）。
     */
    virtual int seekedRecord(shared_ptr<Record>& record) noexcept = 0;
};
}
}
#endif // HUDUN_SQLITE_SEEKEVENTHANDLER_H
