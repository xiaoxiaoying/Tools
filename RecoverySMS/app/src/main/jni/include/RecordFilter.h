#ifndef HUDUN_SQLITE_RECORDFILTER_H
#define HUDUN_SQLITE_RECORDFILTER_H

#include <memory>
using namespace std;

#include "Record.h"

namespace hudun
{
namespace sqlite
{
class RecordFilter
{
public:
    virtual bool check(shared_ptr<Record> const& record) const noexcept = 0;
};
}
}
#endif // HUDUN_SQLITE_RECORDFILTER_H
