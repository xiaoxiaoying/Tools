#ifndef HUDUN_SQLITE_SEEKPROGRESSINDICATOR_H
#define HUDUN_SQLITE_SEEKPROGRESSINDICATOR_H
#include <mutex>
using namespace std;

#include "Timer.h"

namespace hudun
{
namespace sqlite
{
class SeekProgressIndicator final
{
private:
    uint32_t totalPages;
    uint64_t elapsedTime;
    uint32_t seekedPages;
    uint32_t seekedRecords;
    bool completed;

    hudun::common::Timer timer;
    mutex lock;
public:
    SeekProgressIndicator() noexcept;
    SeekProgressIndicator(uint32_t totalPages) noexcept;
    SeekProgressIndicator(SeekProgressIndicator const&) noexcept;
    SeekProgressIndicator& operator=(SeekProgressIndicator const&) noexcept;
    ~SeekProgressIndicator();

    void init(uint32_t totalPages) noexcept;

    void complete() noexcept;

    void inc(uint32_t seekedPages, uint32_t seekedRecords) noexcept;

    void get(uint32_t& totalPages, uint64_t& elapsedTime, uint32_t& seekedPages, uint32_t& seekedRecords, bool& completed) const noexcept;
};
}
}
#endif // HUDUN_SQLITE_SEEKPROGRESSINDICATOR_H
