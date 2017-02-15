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
 * Sqlite���ݿ⣻
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
     * ���캯����
     */
    Database() noexcept;

    /**
     * ��ֹ���ơ�
     */
    Database(Database const&) = delete;

    /**
     * ��ֹ��ֵ��
     */
    Database& operator=(Database const&) = delete;

    /**
     * ����������
     */
    ~Database();

    /**
     * �����ݿ⣬���ʧ�ܣ��׳�CommonException�쳣��
     * @param dbFilename ���ݿ��ļ����ƣ�
     */
    void open(string const& dbFilename);


    /**
     * �����ݿ⣬���ʧ�ܣ����׳�CommonException�쳣�����Բ����¹ʡ�
     * @param dbFilename ���ݿ��ļ����ƣ�
     * @param incident �¹ʣ�
     */
    void open(string const& dbFilename, hudun::common::Incident& incident) noexcept;

    /**
     * �ر����ݿ⡣
     */
    void close();

    /**
     * �ж����ݿ��Ƿ�򿪡�
     */
    bool isOpen() const noexcept;

    /**
     * ��ȡ���ݿ��ļ����ơ�
     * @return ���ݿ��ļ����ơ�
     */
    string getDbFilename() const noexcept;

    /**
     * ��ȡSqlite���ݿ�ҳ���С��
     * @return Sqlite���ݿ�ҳ���С��
     */
    uint32_t getPageSize() const noexcept;

    /**
     * ��ȡSqlite���ݿ�ҳ�泤�ȣ������ݿ�ҳ���С��ȥҳ��β�����������ȡ�
     * @return Sqlite���ݿ�ҳ�泤�ȡ�
     */
    uint16_t getReservedSpacePerPage() const noexcept;

    /**
     * ��ȡSqlite���ݿ�ҳ��������
     * @return Sqlite���ݿ�ҳ��������
     */
    uint32_t getPages() const noexcept;

    /**
     * ��ȡSqlite���ݿ��ı��ַ������롣
     * @return Sqlite���ݿ��ı��ַ������롣
     */
    uint32_t getTextEncoding() const noexcept;

    /**
     * ��ָ��ҳ���ŵ����ݿ�ҳ���ȡ�������С�
     * @param pageNumber ҳ����;
     * @param buff ҳ�滺�棻
     * @param incident (OUT)�¹ʣ�
     */
    void readPage(uint32_t pageNumber, char* buff, hudun::common::Incident& incident) noexcept;

    /**
     * ��ָ����ʼҳ�����Լ�ҳ���������ݿ�ҳ���ȡ�������С�
     * @param pageNumberStart ��ʼҳ����;
     * @param readPageNum ��ȡҳ��������
     * @param buff ҳ�滺�棻
     * @param incident (OUT)�¹ʣ�
     */
    void readPages(uint32_t pageNumberStart, uint32_t readPageNum, char* buff, hudun::common::Incident& incident) noexcept;

    /**
     * ��ȡ�������ݿ����ݡ�
     * @param incident (OUT)�¹ʣ�
     * @return ���ݿ����ݡ�
     */
    shared_ptr<char> getData(hudun::common::Incident& incident) noexcept;
};
ostream& operator<<(ostream&, Database const&) noexcept;
}
}

#endif // HUDUN_SQLITE_DATABASE_H
