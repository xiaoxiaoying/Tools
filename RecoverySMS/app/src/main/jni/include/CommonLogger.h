#ifndef HUDUN_COMMON_COMMONLOGGER_H
#define HUDUN_COMMON_COMMONLOGGER_H

#include <string>
using namespace std;

#include "Logger.h"

namespace hudun
{
namespace common
{
/**
 * 通用日志类；
 *
 * @author Wu Deyou
 * @date 2015-12-16
 * @note 线程安全
 */
class CommonLogger final: public Logger
{
public:
    enum LogMode { TRUNC, APPEND, BACKUP };
private:
    void* native;
    CommonLogger(CommonLogger const&) = delete;
    void operator=(CommonLogger const&) = delete;

public:
    CommonLogger() noexcept;
    ~CommonLogger() noexcept;

    void open(string const& filename, LogMode logMode=LogMode::TRUNC, bool flushAfterWriteln = true);
    void write(char* buff, uint32_t length) noexcept override;
    void writeln(char* buff, uint32_t length) noexcept override;

    void write(string const& info) noexcept override;
    void writeln(string const& info) noexcept override;
    void writeln(string const& date, string const& time, string const& action, string const& info) noexcept override;
    void writeln(string const& filename, string const& functionName, int lineNo, string const& info) noexcept override;
    void writeln(Incident const& incident) noexcept override;
    void writeln(CommonException const& e) noexcept override;
    void flush() noexcept override;
    void close() noexcept;

};
}
}

#endif // HUDUN_COMMON_COMMONLOGGER_H
