#ifndef HUDUN_SQLITE_SQLITERETURNCODE_H
#define HUDUN_SQLITE_SQLITERETURNCODE_H
namespace hudun
{
namespace sqlite
{
class SqliteReturnCode
{
public:
    static const int64_t SUCCEEDED      = 0;
    static const int64_t FAIL_GENERAL   = -1;
    static const int64_t FAIL_OVERFLOW  = -2;
};

}
}
#endif // HUDUN_SQLITE_SQLITERETURNCODE_H
