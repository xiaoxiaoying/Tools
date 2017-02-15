#include <chrono>
#include <string>
#include <sstream>
using namespace std;

#include "../include/Timer.h"
using namespace hudun::common;

Timer::Timer() noexcept
{
}

Timer::Timer(Timer const& other) noexcept
{
    this->startClock = other.startClock;
}

Timer& Timer::operator=(Timer const& other) noexcept
{
    if(this != &other)
    {
        this->startClock = other.startClock;
    }
    return *this;
}

void Timer::start() noexcept
{
    this->startClock = std::chrono::high_resolution_clock::now();
}

uint64_t Timer::elapsed() const noexcept
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->startClock).count();
}

string Timer::format(uint64_t ms) noexcept
{
    uint32_t _ms = ms % 1000;
    uint32_t s = (uint32_t)(ms / 1000);
    uint32_t m = s / 60;
    s %= 60;
    uint32_t h = m / 60;
    m %= 60;

    stringstream ss;


    if(h > 0)
    {
        ss << h << "h";
    }
    if((h > 0) || (m > 0))
    {
        ss << m << "m";
    }
    if((h > 0) || (m > 0) || (s > 0))
    {
        ss << s << "s";
    }

    ss << _ms << "ms";
    return ss.str();
}

string Timer::format(uint64_t ms, uint16_t width) noexcept
{
    string s = format(ms);
    return s.substr(0, width);
}
