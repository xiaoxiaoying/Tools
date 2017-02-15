#include <iostream>
#include <memory>
using namespace std;

#include "../include/Incident.h"
using namespace hudun::common;

#include "../include/RecordSkippedHead.h"
#include "../include/SqliteFacility.h"
using namespace hudun::sqlite;

RecordSkippedHead::RecordSkippedHead() noexcept
{
    this->recordSize = 0;
    this->rowId = 0;
    this->headerSize = 0;
    this->firstFieldType = 0;

    this->recordSizeOffset = 0;
    this->rowIdOffset = 0;
    this->headerSizeOffset = 0;
    this->firstFieldTypeOffset = 0;
    this->minimalOffset = 0;

    this->recordSizeBytes = 0;
    this->rowIdBytes = 0;
    this->headerSizeBytes = 0;
    this->firstFieldTypeBytes = 0;

    this->validHeadItems = RecordSkippedHead::SKIPPEDHEAD_NONE;
}

RecordSkippedHead::RecordSkippedHead(RecordSkippedHead const& other) noexcept
{
//    this->offset = other.offset;
    this->recordSize = other.recordSize;
    this->rowId = other.rowId;
    this->headerSize = other.headerSize;
    this->firstFieldType = other.firstFieldType;

    this->recordSizeOffset = other.recordSizeOffset;
    this->rowIdOffset = other.rowIdOffset;
    this->headerSizeOffset = other.headerSizeOffset;
    this->firstFieldTypeOffset = other.firstFieldTypeOffset;
    this->minimalOffset = other.minimalOffset;

    this->recordSizeBytes = other.recordSizeBytes;
    this->rowIdBytes = other.rowIdBytes;
    this->headerSizeBytes = other.headerSizeBytes;
    this->firstFieldTypeBytes = other.firstFieldTypeBytes;

    this->validHeadItems = other.validHeadItems;
}

RecordSkippedHead& RecordSkippedHead::operator=(RecordSkippedHead const& other) noexcept
{
    if(this != &other)
    {
//        this->offset = other.offset;
        this->recordSize = other.recordSize;
        this->rowId = other.rowId;
        this->headerSize = other.headerSize;
        this->firstFieldType = other.firstFieldType;

        this->recordSizeOffset = other.recordSizeOffset;
        this->rowIdOffset = other.rowIdOffset;
        this->headerSizeOffset = other.headerSizeOffset;
        this->firstFieldTypeOffset = other.firstFieldTypeOffset;
        this->minimalOffset = other.minimalOffset;

        this->recordSizeBytes = other.recordSizeBytes;
        this->rowIdBytes = other.rowIdBytes;
        this->headerSizeBytes = other.headerSizeBytes;
        this->firstFieldTypeBytes = other.firstFieldTypeBytes;

        this->validHeadItems = other.validHeadItems;
    }
    return *this;
}

int64_t RecordSkippedHead::getRecordSize() const noexcept
{
    return this->recordSize;
}

int64_t RecordSkippedHead::getRowId() const noexcept
{
    return this->rowId;
}

int64_t RecordSkippedHead::getHeaderSize() const noexcept
{
    return this->headerSize;
}

int64_t RecordSkippedHead::getFirstFieldType() const noexcept
{
    return this->firstFieldType;
}

uint32_t RecordSkippedHead::getRecordSizeOffset() const noexcept
{
    return this->recordSizeOffset;
}

uint32_t RecordSkippedHead::getRowIdOffset() const noexcept
{
    return this->rowIdOffset;
}

uint32_t RecordSkippedHead::getHeaderSizeOffset() const noexcept
{
    return this->headerSizeOffset;
}

uint32_t RecordSkippedHead::getFirstFieldTypeOffset() const noexcept
{
    return this->firstFieldTypeOffset;
}

uint32_t RecordSkippedHead::getMinimalOffset() const noexcept
{
    return this->minimalOffset;
}

uint8_t RecordSkippedHead::getRecordSizeBytes() const noexcept
{
    return this->recordSizeBytes;
}

uint8_t RecordSkippedHead::getRowIdBytes() const noexcept
{
    return this->rowIdBytes;
}

uint8_t RecordSkippedHead::getHeaderSizeBytes() const noexcept
{
    return this->headerSizeBytes;
}

uint8_t RecordSkippedHead::getFirstFieldTypeBytes() const noexcept
{
    return this->firstFieldTypeBytes;
}

bool RecordSkippedHead::isValid(uint32_t headItems) const noexcept
{
    return ((this->validHeadItems & headItems) == headItems);
}

/**
 * ��ȡ��һ���䳤����������ҳ��ƫ�ƣ�offset������һ���ֽ���Ϊ���һ���ֽڵı��������������ɹ�������varintOffset��varint�����ᱻ�ı䣻
 * @param page ҳ�����ݣ�
 * @param offset ҳ��ƫ�ơ�
 * @param varintOffset ��OUT��������һ���䳤���������ҳ����ʼ��ƫ�ƣ�
 * @param varint ��OUT��������һ���䳤����ֵ��
 * @return �ɹ����
 */
static bool lastVarint(char* page, uint32_t offset, uint32_t& varintOffset, int64_t& varint) noexcept
{
    if(offset == 0) return false;

    // ���ӵ�2���ֽ��Ƿ��λΪ1��
    uint32_t _varintOffset = offset - 1;
    for(uint32_t i = 2; i <= 9; i ++)
    {
        if(i > offset) break;

        char* p = page + (offset - i);
        if((*p & 0x80) == 0x80)  _varintOffset = offset - i;
        else break;
    }

    // ���ͷһ���ֽڣ�
    char* first = page + (offset - 1);
    if(((*first & 0x80) == 0x80) && (offset - _varintOffset != 9)) return false;

    // ȡֵ��
    uint32_t varintBytes;
    Incident incident;
    int64_t _varint = SqliteFacility::parseVarint(page + _varintOffset, offset - _varintOffset, varintBytes, incident);

    if(incident.succeeded())
    {
        varintOffset = _varintOffset;
        varint = _varint;
        return true;
    }
    else return false;
}

shared_ptr<RecordSkippedHead> RecordSkippedHead::parse(char* page, uint32_t offset) noexcept
{
    // ׼����
    shared_ptr<RecordSkippedHead> recordSkippedHead(new RecordSkippedHead());
    recordSkippedHead->minimalOffset = offset;

    // FirstFieldType��
    bool ok = lastVarint(page, offset, recordSkippedHead->firstFieldTypeOffset, recordSkippedHead->firstFieldType);
    if(ok)
    {
        recordSkippedHead->firstFieldTypeBytes = (uint8_t)(offset - recordSkippedHead->firstFieldTypeOffset);
        recordSkippedHead->validHeadItems |= RecordSkippedHead::SKIPPEDHEAD_FIRSTFIELD_TYPE;
        recordSkippedHead->minimalOffset = recordSkippedHead->firstFieldTypeOffset;
    }
    else return shared_ptr<RecordSkippedHead>(new RecordSkippedHead());

    // Header Size;
    ok = lastVarint(page, recordSkippedHead->firstFieldTypeOffset, recordSkippedHead->headerSizeOffset, recordSkippedHead->headerSize);
    if(ok)
    {
        recordSkippedHead->headerSizeBytes = (uint8_t)(recordSkippedHead->firstFieldTypeOffset - recordSkippedHead->headerSizeOffset);
        recordSkippedHead->validHeadItems |= RecordSkippedHead::SKIPPEDHEAD_HEADER_SIZE;
        recordSkippedHead->minimalOffset = recordSkippedHead->headerSizeOffset;
    }
    else  return recordSkippedHead;

    //  rowId;
    ok = lastVarint(page, recordSkippedHead->headerSizeOffset, recordSkippedHead->rowIdOffset, recordSkippedHead->rowId);
    if(ok)
    {
        recordSkippedHead->rowIdBytes = (uint8_t)(recordSkippedHead->headerSizeOffset - recordSkippedHead->rowIdOffset);
        recordSkippedHead->validHeadItems |= RecordSkippedHead::SKIPPEDHEAD_ROWID;
        recordSkippedHead->minimalOffset = recordSkippedHead->rowIdOffset;
    }
    else return recordSkippedHead;

    //  record size;
    ok = lastVarint(page, recordSkippedHead->rowIdOffset, recordSkippedHead->recordSizeOffset, recordSkippedHead->recordSize);
    if(ok)
    {
        recordSkippedHead->recordSizeBytes = (uint8_t)(recordSkippedHead->rowIdOffset - recordSkippedHead->recordSizeOffset);
        recordSkippedHead->validHeadItems |= RecordSkippedHead::SKIPPEDHEAD_RECORD_SIZE;
        recordSkippedHead->minimalOffset = recordSkippedHead->recordSizeOffset;
    }

    return recordSkippedHead;
}

ostream& hudun::sqlite::operator<<(ostream& out, RecordSkippedHead const& recordSkippedHead) noexcept
{
    out << "Minimal Offset: " << recordSkippedHead.getMinimalOffset() << endl;
    if(recordSkippedHead.isValid(RecordSkippedHead::SKIPPEDHEAD_RECORD_SIZE))
    {
        out << "RecordSize: "        << recordSkippedHead.getRecordSize()                << endl;
        out << "RecordSize Offset: " << recordSkippedHead.getRecordSizeOffset()          << endl;
        out << "RecordSize Bytes: "  << (uint32_t)recordSkippedHead.getRecordSizeBytes() << endl;
    }

    if(recordSkippedHead.isValid(RecordSkippedHead::SKIPPEDHEAD_ROWID))
    {
        out << "rowId: "        << recordSkippedHead.getRowId()                << endl;
        out << "rowId Offset: " << recordSkippedHead.getRowIdOffset()          << endl;
        out << "rowId Bytes: "  << (uint32_t)recordSkippedHead.getRowIdBytes() << endl;

    }

    if(recordSkippedHead.isValid(RecordSkippedHead::SKIPPEDHEAD_HEADER_SIZE))
    {
        out << "HeaderSize: "        << recordSkippedHead.getHeaderSize()                << endl;
        out << "HeaderSize Offset: " << recordSkippedHead.getHeaderSizeOffset()          << endl;
        out << "HeaderSize Bytes: "  << (uint32_t)recordSkippedHead.getHeaderSizeBytes() << endl;
    }

    if(recordSkippedHead.isValid(RecordSkippedHead::SKIPPEDHEAD_FIRSTFIELD_TYPE))
    {
        out << "FirstFieldType: "        << recordSkippedHead.getFirstFieldType()                << endl;
        out << "FirstFieldType Offset: " << recordSkippedHead.getFirstFieldTypeOffset()          << endl;
        out << "FirstFieldType Bytes: "  << (uint32_t)recordSkippedHead.getFirstFieldTypeBytes() << endl;
    }

    return out;
}
