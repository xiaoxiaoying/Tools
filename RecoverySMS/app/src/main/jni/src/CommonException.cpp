#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "../include/CommonException.h"
using namespace hudun::common;

CommonException::CommonException() noexcept: exception()
{
    this->code = 0;
    this->lineNo = 0;
}

CommonException::CommonException(int64_t code) noexcept: exception()
{
    this->code = code;
    this->lineNo = 0;
}

CommonException::CommonException(std::string const& message) noexcept: exception()
{
    this->code = 0;
    this->message = message;
    this->lineNo = 0;
}

CommonException::CommonException(int64_t code, string const& message) noexcept: exception()
{
    this->code = code;
    this->message = message;
    this->lineNo = 0;
}

CommonException::CommonException(int64_t code, string const& debugInfo, string const& filename, string const& functionName, uint32_t lineNo) noexcept: exception()
{
    this->code = code;
    this->debugInfo = debugInfo;
    this->filename = filename;
    this->functionName = functionName;
    this->lineNo = lineNo;
}

CommonException::CommonException(CommonException const& other) noexcept: exception()
{
    this->code = other.code;
    this->message = other.message;
    this->debugInfo = other.debugInfo;
    this->filename = other.filename;
    this->functionName = other.functionName;
    this->lineNo = other.lineNo;
}

CommonException& CommonException::operator=(CommonException const& other) noexcept
{
    if(this != &other)
    {
        this->code = other.code;
        this->message = other.message;
        this->debugInfo = other.debugInfo;
        this->filename = other.filename;
        this->functionName = other.functionName;
        this->lineNo = other.lineNo;
    }
    return *this;
}

CommonException::~CommonException()
{
}

int64_t CommonException::getCode() const noexcept
{
    return this->code;
}

string CommonException::getMessage() const noexcept
{
    return this->message;
}

string CommonException::getDebugInfo() const noexcept
{
    return this->debugInfo;
}

string CommonException::getFilename() const noexcept
{
    return this->filename;
}

string CommonException::getFunctionName() const noexcept
{
    return this->functionName;
}

uint32_t CommonException::getLineNo() const noexcept
{
    return this->lineNo;
}

const char* CommonException::what() const noexcept
{
    if(!this->message.empty())
    {
        return this->message.c_str();
    }
    else if(!this->debugInfo.empty())
    {
        return (const char*)this->debugInfo.c_str();
    }
    else
    {
        return "unknown reason!";
    }
}

static string to_hex(int64_t n) noexcept
{
    char buff[32];
    bool negative = false;
    if(n < 0)
    {
        negative = true;
        n = -n;
    }

    uint16_t offset = 0;
    if(negative)
    {
        buff[0] = '-';
        offset = 1;
    }


    uint32_t uh = (n >> 32);
    uint32_t ul = (uint32_t)(n & 0xffffffff);
    if(uh != 0)
    {
        sprintf(buff + offset, "%08X", uh);
        offset += 8;
    }
    sprintf(buff + offset, "%08X", ul);

    return string(buff);
}

ostream& operator<<(ostream& out, CommonException const& e) noexcept
{
    out << "Code: " << to_hex(e.getCode()) << endl;
    out << "Message: " << e.getMessage() << endl;
    out << "DebugInfo: " << e.getDebugInfo() << endl;
    out << "Filename: " << e.getFilename() << endl;
    out << "FunctionName: " << e.getFunctionName() << endl;
    out << "LineNo: " << e.getLineNo() << endl;

    return out;
}




