#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "../include/Incident.h"
using namespace hudun::common;

const int64_t Incident::SUCCEEDED    = 0;
const int64_t Incident::FAIL_GENERAL = -1;
const int64_t Incident::WARN_GENERAL = 1;

Incident::Incident() noexcept
{
    this->lineNo = 0;
    this->code = Incident::SUCCEEDED;
    this->auxiliary = nullptr;
}

Incident::Incident(int64_t code) noexcept
{
    this->lineNo = 0;
    this->code = code;
    this->auxiliary = nullptr;
}

Incident::Incident(int64_t code, string const& message) noexcept
{
    this->lineNo = 0;
    this->code = code;
    this->message = message;
    this->auxiliary = nullptr;
}

Incident::Incident(int64_t code, string const& message, string const& filename, string const& functionName, uint32_t lineNo) noexcept
{
    this->fileName = filename;
    this->functionName = functionName;
    this->lineNo = lineNo;
    this->code = code;
    this->message = message;
    this->auxiliary = nullptr;
}

Incident::Incident(Incident const& other) noexcept
{
    this->fileName = other.fileName;
    this->functionName = other.functionName;
    this->lineNo = other.lineNo;
    this->code = other.code;
    this->message = other.message;
    if(other.auxiliary == nullptr)
    {
        if(this->auxiliary != nullptr)
        {
            delete this->auxiliary;
            this->auxiliary = nullptr;
        }
    }
    else
    {
        if(this->auxiliary == nullptr) this->auxiliary = new map<string, string>();
        else this->auxiliary->clear();
        *(this->auxiliary) = *(other.auxiliary);
    }
}

Incident::Incident(Incident&& other) noexcept
{
    this->fileName = other.fileName;
    this->functionName = other.functionName;
    this->lineNo = other.lineNo;
    this->code = other.code;
    this->message = other.message;
    this->auxiliary = other.auxiliary;
    other.auxiliary = nullptr;
}

Incident& Incident::operator=(Incident const& other) noexcept
{
    if(this != &other)
    {
        this->fileName = other.fileName;
        this->functionName = other.functionName;
        this->lineNo = other.lineNo;
        this->code = other.code;
        this->message = other.message;
        if(other.auxiliary == nullptr)
        {
            if(this->auxiliary != nullptr)
            {
                delete this->auxiliary;
                this->auxiliary = nullptr;
            }
        }
        else
        {
            if(this->auxiliary == nullptr) this->auxiliary = new map<string, string>();
            else this->auxiliary->clear();
            *(this->auxiliary) = *(other.auxiliary);
        }
    }
    return *this;
}

Incident& Incident::operator=(Incident&& other) noexcept
{
    if(this != &other)
    {
        this->fileName = other.fileName;
        this->functionName = other.functionName;
        this->lineNo = other.lineNo;
        this->code = other.code;
        this->message = other.message;
        this->auxiliary = other.auxiliary;
        other.auxiliary = nullptr;
    }
    return *this;
}


Incident& Incident::operator=(int64_t code) noexcept
{
    this->fileName = "";
    this->functionName = "";
    this->lineNo = 0;
    this->code = code;
    this->message = "";
    if(this->auxiliary != nullptr)
    {
        delete this->auxiliary;
        this->auxiliary = nullptr;
    }
    return *this;
}

Incident::~Incident()
{
    if(this->auxiliary != nullptr) delete this->auxiliary;
}

void Incident::set(int64_t code, string const& message, string const& fileName, string const& functionName, uint32_t lineNo) noexcept
{
    this->fileName = fileName;
    this->functionName = functionName;
    this->lineNo = lineNo;
    this->code = code;
    this->message = message;

    if(this->auxiliary != nullptr)
    {
        delete this->auxiliary;
        this->auxiliary = nullptr;
    }
}
void Incident::anchor(string const& fileName, string const& functionName, uint32_t lineNo) noexcept
{
    this->fileName = fileName;
    this->functionName = functionName;
    this->lineNo = lineNo;
}

void Incident::set(string const& name, string value) noexcept
{
    if(value.empty()) return;
    if(this->auxiliary == nullptr)
    {
        this->auxiliary = new map<string, string>();
    }
    (*(this->auxiliary))[name] = value;
}
namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream ss;
        ss << n ;
        return ss.str() ;
    }
}
void Incident::set(string const& name, int64_t value) noexcept
{
    if(this->auxiliary == nullptr)
    {
        this->auxiliary = new map<string, string>();
    }
    (*(this->auxiliary))[name] = to_string(value);
}

void Incident::reset() noexcept
{
    this->fileName = "";
    this->functionName = "";
    this->lineNo = 0;
    this->code = Incident::SUCCEEDED;
    this->message = "";
    if(this->auxiliary != nullptr)
    {
        delete this->auxiliary;
        this->auxiliary = nullptr;
    }
}

bool Incident::succeeded() const noexcept
{
    return (this->code == Incident::SUCCEEDED);
}

string Incident::get(string const& name) const noexcept
{
    if(this->auxiliary == nullptr) return "";
    map<string, string>::const_iterator it = this->auxiliary->find(name);
    if(it == this->auxiliary->end())
    {
        return "";
    }
    else
    {
        return it->second;
    }
}

int64_t Incident::getCode() const noexcept
{
    return this->code;
}

string Incident::getMessage() const noexcept
{
    return this->message;
}

string Incident::getFilename() const noexcept
{
    return this->fileName;
}

string Incident::getFunctionName() const noexcept
{
    return this->functionName;
}

uint32_t Incident::getLineNo() const noexcept
{
    return this->lineNo;
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

string Incident::toString(string separator) const noexcept
{
    stringstream ss;

    // fileName;
    ss << "FileName: " << this->fileName << separator;

    // functionName;
    ss << "FunctionName: " << this->functionName << separator;

    // lineNo;
    ss << "LineNo: " << this->lineNo << separator;

    // code;
    ss << "Code: " << to_hex(this->code) << separator;

    // message;
    ss << "Message: " << this->message;

    // auxiliary;
    if(this->auxiliary != nullptr)
    {
        for (map<string, string>::const_iterator it = this->auxiliary->begin(); it != this->auxiliary->end(); ++ it)
        {
            ss << separator;
            ss << it->first << ": " << it->second;
        }
    }
    return ss.str();
}
