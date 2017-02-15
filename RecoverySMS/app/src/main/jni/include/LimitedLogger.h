#ifndef HUDUN_COMMON_LIMITEDLOGGER_H
#define HUDUN_COMMON_LIMITEDLOGGER_H

#include <string>
using namespace std;

#include "CommonException.h"
#include "Incident.h"
#include "Logger.h"

namespace hudun
{
namespace common
{
/**
 * 有限数量日志类；
 *
 * @author Wu Deyou
 * @date 2015-12-16
 * @note 线程安全
 */
class LimitedLogger: public Logger
{
private:
    uint32_t maxLogItems;
    uint32_t logItems;
    Logger* logger;

public:
    LimitedLogger(Logger* logger, uint32_t maxLogItems) noexcept;
    virtual ~LimitedLogger() noexcept;

    void write(char* buff, uint32_t length) noexcept override;
    void writeln(char* buff, uint32_t length) noexcept override;

    void write(string const& info) noexcept override;
    void writeln(string const& info) noexcept override;
    void writeln(string const& date, string const& time, string const& action, string const& info) noexcept override;
    void writeln(string const& filename, string const& functionName, int lineNo, string const& info) noexcept override;
    void writeln(Incident const& incident) noexcept override;
    void writeln(CommonException const& e) noexcept override;

    void flush() noexcept override;
};
}
}

#endif // HUDUN_COMMON_LIMITEDLOGGER_H
