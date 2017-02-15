#ifndef HUDUN_SQLITE_DATABASE_H
#define HUDUN_SQLITE_DATABASE_H
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
using namespace std;

#include "Incident.h"

namespace hudun
{
namespace sqlite
{
/**
 * Sqlite数据库；
 *
 * @author Wu Deyou
 * @date 2015-12-21
 */
class Database
{
public:
    enum TextEncoding: uint32_t
    {
        UTF_8       = 1,
        UTF_16LE    = 2,
        UTF_16BE    = 3
    };

private:
    string dbFilename;
    uint32_t pageSize;
    uint16_t reservedSpacePerPage;
    uint32_t pages;
    uint32_t textEncoding;
    ifstream dbStream;
    shared_ptr<char> data;

public:
    /**
     * 构造函数。
     */
    Database() noexcept;

    /**
     * 禁止复制。
     */
    Database(Database const&) = delete;

    /**
     * 禁止赋值。
     */
    Database& operator=(Database const&) = delete;

    /**
     * 析构函数。
     */
    ~Database();

    /**
     * 打开数据库，如果失败，抛出CommonException异常。
     * @param dbFilename 数据库文件名称；
     */
    void open(string const& dbFilename);


    /**
     * 打开数据库，如果失败，不抛出CommonException异常，代以产生事故。
     * @param dbFilename 数据库文件名称；
     * @param incident 事故；
     */
    void open(string const& dbFilename, hudun::common::Incident& incident) noexcept;

    /**
     * 关闭数据库。
     */
    void close();

    /**
     * 判断数据库是否打开。
     */
    bool isOpen() const noexcept;

    /**
     * 获取数据库文件名称。
     * @return 数据库文件名称。
     */
    string getDbFilename() const noexcept;

    /**
     * 获取Sqlite数据库页面大小。
     * @return Sqlite数据库页面大小。
     */
    uint32_t getPageSize() const noexcept;

    /**
     * 获取Sqlite数据库页面长度，即数据库页面大小减去页面尾部保留区长度。
     * @return Sqlite数据库页面长度。
     */
    uint16_t getReservedSpacePerPage() const noexcept;

    /**
     * 获取Sqlite数据库页面数量。
     * @return Sqlite数据库页面数量。
     */
    uint32_t getPages() const noexcept;

    /**
     * 获取Sqlite数据库文本字符集编码。
     * @return Sqlite数据库文本字符集编码。
     */
    uint32_t getTextEncoding() const noexcept;

    /**
     * 将指定页面编号的数据库页面读取到缓存中。
     * @param pageNumber 页面编号;
     * @param buff 页面缓存；
     * @param incident (OUT)事故；
     */
    void readPage(uint32_t pageNumber, char* buff, hudun::common::Incident& incident) noexcept;

    /**
     * 将指定起始页面编号以及页面数量数据库页面读取到缓存中。
     * @param pageNumberStart 起始页面编号;
     * @param readPageNum 读取页面数量；
     * @param buff 页面缓存；
     * @param incident (OUT)事故；
     */
    void readPages(uint32_t pageNumberStart, uint32_t readPageNum, char* buff, hudun::common::Incident& incident) noexcept;

    /**
     * 获取整个数据库内容。
     * @param incident (OUT)事故；
     * @return 数据库内容。
     */
    shared_ptr<char> getData(hudun::common::Incident& incident) noexcept;
};
ostream& operator<<(ostream&, Database const&) noexcept;
}
}

#endif // HUDUN_SQLITE_DATABASE_H
