#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
using namespace std;

#include "../include/CommonLogger.h"
#include "../include/CommonException.h"
using namespace hudun::common;

class _NativeCommonLogger
{
private:
    friend CommonLogger;
    mutex lock;
    ofstream* logStream = nullptr;
    bool flushAfterWriteln = true;

    int open(const string& filename, CommonLogger::LogMode logMode, bool flushAfterWriteln) noexcept
    {
        if(this->logStream != nullptr) return -1;
        if(logMode == CommonLogger::LogMode::BACKUP) return -2;
        this->logStream = new ofstream(filename.c_str(), ios::out | ios::trunc | ios::binary);
        if(this->logStream == nullptr) return -3;
        this->flushAfterWriteln = flushAfterWriteln;
        return 0;
    }

    void write(char* buff, uint32_t length) noexcept
    {
        if(this->logStream == nullptr) return;

        lock.lock();
        this->logStream->write(buff, length);
        lock.unlock();
    }

    void writeln(char* buff, uint32_t length) noexcept
    {
        if(this->logStream == nullptr) return;

        lock.lock();
        this->logStream->write(buff, length);
        *(this->logStream) << endl;
        lock.unlock();
    }

    void write(const string& info) noexcept
    {
        if(this->logStream == nullptr) return;

        lock.lock();
        *(this->logStream) << info;
        lock.unlock();
    }

    void writeln(const string& info) noexcept
    {
        if(this->logStream == nullptr) return;
        lock.lock();
        *(this->logStream) << info << endl;
        if(this->flushAfterWriteln) this->logStream->flush();
        lock.unlock();
    }

    void writeln(string const& date, string const& time, string const& action, string const& info) noexcept
    {
        if(this->logStream == nullptr) return;
        lock.lock();
        // date & time
        *(this->logStream) << date << " " << time;

        // action
        *(this->logStream) << " " << action;

        // info
        *(this->logStream) << "\t" << info << endl;
        if(this->flushAfterWriteln) this->logStream->flush();
        lock.unlock();
    }

    void writeln(string const& filename, string const& functionName, int lineNo, string const& info) noexcept
    {
        if(this->logStream == nullptr) return;

        lock.lock();

        // info
        *(this->logStream) << info;

        // filename
        *(this->logStream) << "\t" << filename;

        // functionName
        *(this->logStream) << "|" << functionName;

        // lineNo
        *(this->logStream) << "|" << dec << lineNo << endl;

        if(this->flushAfterWriteln) this->logStream->flush();
        lock.unlock();
    }


    void writeln(Incident const& incident) noexcept
    {
        writeln(incident.getFilename(), incident.getFunctionName(), incident.getLineNo(),  incident.getMessage());
    }

    void writeln(CommonException const& e) noexcept
    {
        writeln(e.getFilename(), e.getFunctionName(), e.getLineNo(),  e.getDebugInfo());
    }

    void flush() noexcept
    {
        if(this->logStream != nullptr)
        {
            lock.lock();
            this->logStream->flush();
            lock.unlock();
        }
    }

    void close() noexcept
    {
        if(this->logStream != nullptr)
        {
            lock.lock();
            this->logStream->close();
            this->logStream = nullptr;
            lock.unlock();
        }
    }

    ~_NativeCommonLogger() noexcept
    {
        close();
    }
};

CommonLogger::CommonLogger() noexcept
{
    this->native = new _NativeCommonLogger();
}

CommonLogger::~CommonLogger() noexcept
{
    delete (_NativeCommonLogger*)this->native;
}

void CommonLogger::open(string const& filename, LogMode logMode, bool flushAfterWriteln)
{
    int returnCode;
    ((_NativeCommonLogger*)this->native)->lock.lock();
    returnCode = ((_NativeCommonLogger*)this->native)->open(filename, logMode, flushAfterWriteln);
    ((_NativeCommonLogger*)this->native)->lock.unlock();

    if(returnCode == -1)
    {
        throw CommonException(CommonException::FAIL_GENERAL, "The CommonLogger instance has been opened!", __FILE__, __func__, __LINE__);
    }
    if(returnCode == -2)
    {
        throw CommonException(CommonException::FAIL_GENERAL, "logger::LogMode::BACKUP has NOT support", __FILE__, __func__, __LINE__);
    }
    if(returnCode == -3)
    {
        throw CommonException(CommonException::FAIL_GENERAL, "open log file[" + filename + "] fail!", __FILE__, __func__, __LINE__);
    }
}

void CommonLogger::write(char* buff, uint32_t length) noexcept
{
    ((_NativeCommonLogger*)this->native)->write(buff, length);
}

void CommonLogger::writeln(char* buff, uint32_t length) noexcept
{
    ((_NativeCommonLogger*)this->native)->writeln(buff, length);
}

void CommonLogger::write(const string& info) noexcept
{
    ((_NativeCommonLogger*)this->native)->write(info);
}

void CommonLogger::writeln(const string& info) noexcept
{
    ((_NativeCommonLogger*)this->native)->writeln(info);
}

void CommonLogger::writeln(string const& date, string const& time, string const& action, string const& info) noexcept
{
    ((_NativeCommonLogger*)this->native)->writeln(date, time, action, info);
}

void CommonLogger::writeln(string const& filename, string const& functionName, int lineNo, string const& info) noexcept
{
    ((_NativeCommonLogger*)this->native)->writeln(filename, functionName, lineNo, info);
}

void CommonLogger::writeln(Incident const& incident) noexcept
{
    ((_NativeCommonLogger*)this->native)->writeln(incident);
}

void CommonLogger::writeln(CommonException const& e) noexcept
{
    ((_NativeCommonLogger*)this->native)->writeln(e);
}

void CommonLogger::flush() noexcept
{
    ((_NativeCommonLogger*)this->native)->flush();
}

void CommonLogger::close() noexcept
{
    ((_NativeCommonLogger*)this->native)->close();
}




