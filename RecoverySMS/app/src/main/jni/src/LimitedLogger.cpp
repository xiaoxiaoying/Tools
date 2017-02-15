#include <string>
using namespace std;

#include "../include/CommonException.h"
#include "../include/Incident.h"
#include "../include/LimitedLogger.h"
using namespace hudun::common;

LimitedLogger::LimitedLogger(Logger* logger, uint32_t maxLogItems) noexcept
{
    this->logger = logger;
    this->maxLogItems = maxLogItems;
    this->logItems = 0;
}

LimitedLogger::~LimitedLogger() noexcept
{
}

static int beforeWrite(uint32_t maxLogItems, uint32_t& logItems) noexcept
{
    if(logItems >= maxLogItems) return 2;

    logItems ++;

    if(logItems == maxLogItems) return 1; else return 0;
}

void LimitedLogger::write(char* buff, uint32_t length) noexcept
{
    int b = beforeWrite(this->maxLogItems, this->logItems);
    if(b == 0)
    {
        this->logger->write(buff, length);
    }
    else if(b == 1)
    {
        this->logger->writeln(buff, length);
        this->logger->writeln("\tSimilar type information may be has more ... , ");
    }
}

void LimitedLogger::writeln(char* buff, uint32_t length) noexcept
{
    int b = beforeWrite(this->maxLogItems, this->logItems);
    if(b == 2) return;
    this->logger->writeln(buff, length);
    if(b == 1) this->logger->writeln("\tSimilar type information may be has more ... , ");
}

void LimitedLogger::write(string const& info) noexcept
{
    int b = beforeWrite(this->maxLogItems, this->logItems);
    if(b == 0)
    {
        this->logger->write(info);
    }
    else if(b == 1)
    {
        this->logger->writeln(info);
        this->logger->writeln("\tSimilar type information may be has more ... , ");
    }
}

void LimitedLogger::writeln(string const& info) noexcept
{
    int b = beforeWrite(this->maxLogItems, this->logItems);
    if(b == 2) return;
    this->logger->writeln(info);
    if(b == 1) this->logger->writeln("\tSimilar type information may be has more ... , ");
}

void LimitedLogger::writeln(string const& date, string const& time, string const& action, string const& info) noexcept
{
    int b = beforeWrite(this->maxLogItems, this->logItems);
    if(b == 2) return;
    this->logger->writeln(date, time, action, info);
    if(b == 1) this->logger->writeln("\tSimilar type information may be has more ... , ");
}

void LimitedLogger::writeln(string const& filename, string const& functionName, int lineNo, string const& info) noexcept
{
    int b = beforeWrite(this->maxLogItems, this->logItems);
    if(b == 2) return;
    this->logger->writeln(filename, functionName, lineNo, info);
    if(b == 1) this->logger->writeln("\tSimilar type information may be has more ... , ");
}

void LimitedLogger::writeln(Incident const& incident) noexcept
{
    int b = beforeWrite(this->maxLogItems, this->logItems);
    if(b == 2) return;
    this->logger->writeln(incident);
    if(b == 1) this->logger->writeln("\tSimilar type information may be has more ... , ");
}

void LimitedLogger::writeln(CommonException const& e) noexcept
{
    int b = beforeWrite(this->maxLogItems, this->logItems);
    if(b == 2) return;
    this->logger->writeln(e);
    if(b == 1) this->logger->writeln("\tSimilar type information may be has more ... , ");
}

void LimitedLogger::flush() noexcept
{
    this->flush();
}
