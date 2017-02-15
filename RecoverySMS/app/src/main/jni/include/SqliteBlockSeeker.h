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
 * Sqlite数据库块搜索器；
 * 在指定的数据块中，搜索Sqlite数据库指定表的记录；
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
     * 构造函数；
     * @param table 搜索目标数据库表对象；
     * @param pageLength Qqlite数据库页面长度；
     * @param recordFilter 记录过滤器对象指针。
     */
    SqliteBlockSeeker(uint32_t pageSize, uint32_t pageLength, RecordInspector* recordInspector, RecordFilter* recordFilter) noexcept;

    /**
     * 复制构造函数；
     */
    SqliteBlockSeeker(SqliteBlockSeeker const&) noexcept;

    /**
     * 对象赋值操作；
     */
    SqliteBlockSeeker& operator=(SqliteBlockSeeker const&) noexcept;

    /**
     * 析构函数；
     */
    ~SqliteBlockSeeker();

     /**
     * 搜索记录函数；
     * @param pageBlock 数据块；
     * @param pageNumberStart 数据块开始页面编号；
     * @param pageNum 数据块包含的页面数；
     * @param seekEventHandler 搜索事件处理器对象指针；
     * @return 搜索到的记录数量。
     * @note 如果外部注入的recordFilter与seekEventHandler多线程安全，则本方法也是线程安全的。
     */
    uint32_t seek(char* pageBlock, uint32_t pageNumberStart, uint32_t pageNum, SeekProgressIndicator* seekProgressIndicator, SeekEventHandler* seekEventHandler) noexcept;
};
}
}

#endif // HUDUN_SQLITE_SQLITEBLOCKSEEKER_H

