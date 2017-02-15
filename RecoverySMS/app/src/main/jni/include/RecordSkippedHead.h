#ifndef HUDUN_SQLITE_RECORDSKIPPEDHEAD_H
#define HUDUN_SQLITE_RECORDSKIPPEDHEAD_H
#include <iostream>
#include <memory>
using namespace std;

namespace hudun
{
namespace sqlite
{
class RecordSkippedHead
{
public:
    static const uint32_t SKIPPEDHEAD_NONE            = 0;
    static const uint32_t SKIPPEDHEAD_RECORD_SIZE     = 0x0001;
    static const uint32_t SKIPPEDHEAD_ROWID           = 0x0002;
    static const uint32_t SKIPPEDHEAD_HEADER_SIZE     = 0x0004;
    static const uint32_t SKIPPEDHEAD_FIRSTFIELD_TYPE = 0x0008;

protected:
    int64_t recordSize;
    int64_t rowId;
    int64_t headerSize;
    int64_t firstFieldType;

    uint32_t recordSizeOffset;
    uint32_t rowIdOffset;
    uint32_t headerSizeOffset;
    uint32_t firstFieldTypeOffset;
    uint32_t minimalOffset;

    uint8_t recordSizeBytes;
    uint8_t rowIdBytes;
    uint8_t headerSizeBytes;
    uint8_t firstFieldTypeBytes;

    uint32_t validHeadItems;

public:
    RecordSkippedHead() noexcept;
    RecordSkippedHead(RecordSkippedHead const&) noexcept;
    RecordSkippedHead& operator=(RecordSkippedHead const&) noexcept;

    int64_t getRecordSize() const noexcept;
    int64_t getRowId() const noexcept;
    int64_t getHeaderSize() const noexcept;
    int64_t getFirstFieldType() const noexcept;

    uint32_t getRecordSizeOffset() const noexcept;
    uint32_t getRowIdOffset() const noexcept;
    uint32_t getHeaderSizeOffset() const noexcept;
    uint32_t getFirstFieldTypeOffset() const noexcept;
    uint32_t getMinimalOffset() const noexcept;

    uint8_t getRecordSizeBytes() const noexcept;
    uint8_t getRowIdBytes() const noexcept;
    uint8_t getHeaderSizeBytes() const noexcept;
    uint8_t getFirstFieldTypeBytes() const noexcept;


    bool isValid(uint32_t headItems) const noexcept;
    static shared_ptr<RecordSkippedHead> parse(char* page, uint32_t offset) noexcept;
};
ostream& operator<<(ostream&, RecordSkippedHead const&) noexcept;
}
}

#endif // HUDUN_SQLITE_RECORDSKIPPEDHEAD_H


