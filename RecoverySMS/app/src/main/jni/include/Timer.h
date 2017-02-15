#ifndef HUDUN_COMMON_TIMER_H
#define HUDUN_COMMON_TIMER_H

#include <chrono>
#include <string>
using namespace std;

namespace hudun
{
namespace common
{
class Timer
{
private:
    chrono::time_point<chrono::high_resolution_clock> startClock = chrono::high_resolution_clock::now();

public:
    Timer() noexcept;
    Timer(Timer const&) noexcept;
    Timer& operator=(Timer const&) noexcept;
    void start() noexcept;
    uint64_t elapsed() const noexcept;
    static string format(uint64_t ms) noexcept;
    static string format(uint64_t ms, uint16_t width) noexcept;

};
}
}

#endif // HUDUN_COMMON_TIMER_H
