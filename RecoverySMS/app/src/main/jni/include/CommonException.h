#ifndef HUDUN_COMMON_COMMONEXCEPTION_H
#define HUDUN_COMMON_COMMONEXCEPTION_H
#include <iostream>
#include <exception>
#include <string>
using namespace std;

namespace hudun
{
namespace common
{
class CommonException: public exception
{
public:
    static const int64_t FAIL_GENERAL = -1;
    static const int64_t CAUTION_GENERAL = 1;

protected:
    int64_t code;
    string message;
    string debugInfo;
    string filename;
    string functionName;
    uint32_t lineNo;

public:
    CommonException() noexcept;
    CommonException(int64_t code) noexcept;
    CommonException(string const& message) noexcept;
    CommonException(int64_t code, string const& message) noexcept;
    CommonException(int64_t code, string const& debugInfo, string const& filename, string const& functionName, uint32_t lineNo) noexcept;
    CommonException(CommonException const&) noexcept;
    CommonException& operator=(CommonException const&) noexcept;
    ~CommonException();

    int64_t getCode() const noexcept;
    string getMessage() const noexcept;
    string getDebugInfo() const noexcept;
    string getFilename() const noexcept;
    string getFunctionName() const noexcept;
    uint32_t getLineNo() const noexcept;

    const char* what() const noexcept override;
};
}
}
ostream& operator<<(ostream&, hudun::common::CommonException const&) noexcept;

#endif // HUDUN_COMMON_COMMONEXCEPTION_H
