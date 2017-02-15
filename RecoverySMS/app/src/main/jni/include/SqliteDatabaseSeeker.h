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
 * Sqlite数据库搜索器；
 * 在Sqlite数据库（文件）中，搜索指定表的记录；
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
     * 构造函数；
     * @param database Sqlite数据库对象指针；
     * @param table 搜索目标数据库表对象指针；
     * @param recordFilter 记录过滤器对象指针。
     * @param processOverflowRecord 是否处理溢出记录；
     */
    SqliteDatabaseSeeker(Database& database, hudun::sqlite::schema::Table const& table, RecordFilter* recordFilter, bool processOverflowRecord) noexcept;

    /**
     * 复制构造函数；
     */
    SqliteDatabaseSeeker(SqliteDatabaseSeeker const&) noexcept;

    /**
     * 对象赋值操作；
     */
    SqliteDatabaseSeeker& operator=(SqliteDatabaseSeeker const&) noexcept;

    /**
     * 析构函数；
     */
    ~SqliteDatabaseSeeker();

     /**
     * 搜索记录函数（共享搜索事件处理器版本）；
     * @param threadNum 搜索线程数量
     * @param seekProgressIndicator 进度指示器（可空）；
     * @param seekEventHandler 搜索事件处理器（不可为空），需多线程安全；
     * @param incident 事故；
     * @return 搜索到的记录数量。
     */
    uint32_t seek(uint32_t threadNum, SeekProgressIndicator* seekProgressIndicator, SeekEventHandler* seekEventHandler, hudun::common::Incident& incident) noexcept;

     /**
     * 搜索记录函数（非共享搜索事件处理器版本）；
     * @param threadNum 搜索线程数量
     * @param seekProgressIndicator 进度指示器（可空）；
     * @param seekEventHandler 搜索事件处理器数组，每个线程单独使用一个搜索处理器；
     * @param incident 事故；
     * @return 搜索到的记录数量。
     */
    uint32_t seek(uint32_t threadNum, SeekProgressIndicator* seekProgressIndicator, SeekEventHandler** seekEventHandlers, hudun::common::Incident& incident) noexcept;
};
}
}

#endif // HUDUN_SQLITE_SQLITEDATABASESEEKER_H


