#ifndef HUDUN_COMMON_LOGGER_H
#define HUDUN_COMMON_LOGGER_H
#include <string>
using namespace std;

#include "CommonException.h"
#include "Incident.h"

namespace hudun
{
namespace common
{
/**
 * 日志接口；
 *
 * @author Wu Deyou
 * @date 2015-12-16
 */
class Logger
{
public:
    virtual void write(char* buff, uint32_t length) noexcept = 0;
    virtual void writeln(char* buff, uint32_t length) noexcept = 0;
    virtual void write(string const& info) noexcept = 0;
    virtual void writeln(string const& info) noexcept = 0;
    virtual void writeln(string const& date, string const& time, string const& action, string const& info) noexcept = 0;
    virtual void writeln(string const& filename, string const& functionName, int lineNo, string const& info) noexcept= 0;
    virtual void writeln(Incident const& incident) noexcept = 0;
    virtual void writeln(CommonException const& e) noexcept = 0;
    virtual void flush() noexcept = 0;

};
}
}

#endif // HUDUN_COMMON_LOGGER_H
