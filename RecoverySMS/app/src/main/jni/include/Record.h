#ifndef HUDUN_SQLITE_RECORD_H
#define HUDUN_SQLITE_RECORD_H
#include <string>
#include <vector>
#include <map>
#include <memory>
using namespace std;

#include "FieldValue.h"
#include "RecordSkippedHead.h"

namespace hudun
{
namespace sqlite
{
class Record
{
private:
    uint32_t pageNumber;
    shared_ptr<RecordSkippedHead> recordSkippedHead;

    map<uint32_t, shared_ptr<FieldValue>> fieldIndexToFieldValueMap;
    map<string, shared_ptr<FieldValue>> fieldNameToFieldValueMap;
public:
    Record() noexcept;
    Record(Record const&) noexcept;
    Record& operator=(Record const&) noexcept;

    void add(shared_ptr<FieldValue> const& fieldValue) noexcept;
    void clear() noexcept;

    uint32_t getFieldValueNum() const noexcept;
    vector<uint32_t> getAllFieldIndexes() const noexcept;
    vector<string> getAllFieldNames() const noexcept;
    const shared_ptr<FieldValue> getFieldValueByFieldIndex(uint32_t fieldIndex) const noexcept;
    const shared_ptr<FieldValue> getFieldValueByFieldName(string const& fieldName) const noexcept;

    void setPageNumber(uint32_t pageNumber) noexcept;
    uint32_t getPageNumber() const noexcept;

    void setRecordSkippedHead(shared_ptr<RecordSkippedHead> const& recordSkippedHead) noexcept;
    shared_ptr<RecordSkippedHead> getRecordSkippedHead() const noexcept;
};
ostream& operator<<(ostream& out, Record const& record) noexcept;
}
}

#endif // HUDUN_SQLITE_RECORD_H
