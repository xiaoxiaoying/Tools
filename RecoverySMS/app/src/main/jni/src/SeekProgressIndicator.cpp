#include <mutex>
using namespace std;

#include "../include/Timer.h"
using namespace hudun::common;

#include "../include/SeekProgressIndicator.h"
using namespace hudun::sqlite;

SeekProgressIndicator::SeekProgressIndicator() noexcept
{
    this->totalPages = 0;
    this->elapsedTime = 0;
    this->seekedPages = 0;
    this->seekedRecords = 0;
    this->completed = false;
}
SeekProgressIndicator::SeekProgressIndicator(uint32_t totalPages) noexcept
{
    this->totalPages = totalPages;
    this->elapsedTime = 0;
    this->seekedPages = 0;
    this->seekedRecords = 0;
    this->completed = false;
}

SeekProgressIndicator::SeekProgressIndicator(SeekProgressIndicator const& other) noexcept
{
    this->totalPages = other.totalPages;
    this->elapsedTime = other.elapsedTime;
    this->seekedPages = other.seekedPages;
    this->seekedRecords = other.seekedRecords;
    this->completed = other.completed;
    this->timer = other.timer;
}

SeekProgressIndicator& SeekProgressIndicator::operator=(SeekProgressIndicator const& other) noexcept
{
    if(this != &other)
    {
        this->lock.lock();
        this->totalPages = other.totalPages;
        this->elapsedTime = other.elapsedTime;
        this->seekedPages = other.seekedPages;
        this->seekedRecords = other.seekedRecords;
        this->completed = other.completed;
        this->timer = other.timer;
        this->lock.unlock();

    }
    return *this;
}

SeekProgressIndicator::~SeekProgressIndicator()
{
}

void SeekProgressIndicator::init(uint32_t totalPages) noexcept
{
    this->lock.lock();
    this->totalPages = totalPages;
    this->elapsedTime = 0;
    this->seekedPages = 0;
    this->seekedRecords = 0;
    this->completed = false;
    this->timer.start();
    this->lock.unlock();
}

void SeekProgressIndicator::complete() noexcept
{
    this->lock.lock();
    this->completed = true;
    this->lock.unlock();
}

void SeekProgressIndicator::inc(uint32_t seekedPages, uint32_t seekedRecords) noexcept
{
    this->lock.lock();
    this->elapsedTime = this->timer.elapsed();
    this->seekedPages += seekedPages;
    this->seekedRecords += seekedRecords;
    this->lock.unlock();
}

void SeekProgressIndicator::get(uint32_t& totalPages, uint64_t& elapsedTime, uint32_t& seekedPages, uint32_t& seekedRecords, bool& completed) const noexcept
{
    totalPages = this->totalPages;
    elapsedTime = this->elapsedTime;
    seekedPages = this->seekedPages;
    seekedRecords = this->seekedRecords;
    completed = this->completed;
}
