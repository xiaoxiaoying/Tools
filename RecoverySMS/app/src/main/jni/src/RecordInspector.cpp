#include <iostream>
#include <string>
#include <memory>
#include <vector>
using namespace std;

#include "../include/Incident.h"
#include "../include/CommonFacility.h"
using namespace hudun::common;

#include "../include/Field.h"
#include "../include/Table.h"
using namespace hudun::sqlite::schema;

#include "../include/FieldTypeInspector.h"
#include "../include/Record.h"
#include "../include/RecordInspector.h"
#include "../include/RecordSkippedHead.h"
#include "../include/SqliteFacility.h"
#include "../include/SqliteReturnCode.h"
#include "../include/Strto.h"

using namespace hudun::sqlite;

// ����Table B-Tree��¼����С����󱾵ش洢�ֽ�������
static void calculateLocalIntervalOfTBTRecord(uint32_t usableSize, uint8_t minEmbeddedFraction, uint32_t& minLocalSize, uint32_t& maxLocalSize, Incident& incident) noexcept
{
    // ������С���ش洢���ȣ�
    if(usableSize <= 35)
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "UsableSize[" + to_string(usableSize) + "] <= 35!", __FILE__, __func__, __LINE__);
        return;
    }
    if((usableSize - 12) * minEmbeddedFraction <= 5865)
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "UsableSize[" + to_string(usableSize) + "] and MinEmbeddedFraction[" + to_string(minEmbeddedFraction) + "] cannot make '(usableSize - 12) * minEmbeddedFraction <= 5856'", __FILE__, __func__, __LINE__);
        return;
    }
    minLocalSize = (usableSize - 12) * minEmbeddedFraction / 255 - 23;
    maxLocalSize = usableSize - 35;
    incident.reset();
    return;
}

RecordInspector::RecordInspector(Database& database, hudun::sqlite::schema::Table const& table, bool processOverflowRecord) noexcept
{
    this->database = &database;
    this->table = &table;
    this->processOverflowRecord = processOverflowRecord;

    this->integerPrimaryKeyFieldIndex = -1;

    this->totalPages = database.getPages();
    this->pageSize = database.getPageSize();
    this->pageLength = database.getPageSize() - database.getReservedSpacePerPage();
    this->minEmbeddedFraction = 32;

    Incident incident;
    calculateLocalIntervalOfTBTRecord(this->pageLength, this->minEmbeddedFraction, this->minLocalSize, this->maxLocalSize, incident);
    if(!incident.succeeded())
    {
        this->minLocalSize = 0;
        this->maxLocalSize = 0;
    }
}

RecordInspector::RecordInspector(RecordInspector const& other) noexcept
{
    this->database = other.database;
    this->table = other.table;
    this->processOverflowRecord = other.processOverflowRecord;

    this->integerPrimaryKeyFieldIndex = other.integerPrimaryKeyFieldIndex;
    this->totalPages = other.totalPages;
    this->pageSize = other.pageSize;
    this->pageLength = other.pageLength;
    this->minEmbeddedFraction = other.minEmbeddedFraction;
    this->minLocalSize = other.minLocalSize;
    this->maxLocalSize = other.maxLocalSize;
}

RecordInspector& RecordInspector::operator=(RecordInspector const& other) noexcept
{
    if(this != &other)
    {
        this->database = other.database;
        this->table = other.table;
        this->processOverflowRecord = other.processOverflowRecord;

        this->integerPrimaryKeyFieldIndex = other.integerPrimaryKeyFieldIndex;
        this->totalPages = other.totalPages;
        this->pageSize = other.pageSize;
        this->pageLength = other.pageLength;
        this->minEmbeddedFraction = other.minEmbeddedFraction;
        this->minLocalSize = other.minLocalSize;
        this->maxLocalSize = other.maxLocalSize;
    }
    return *this;
}

void RecordInspector::pushBack(FieldTypeInspector const& fieldTypeInspector) noexcept
{
    this->fieldTypeInspectors.push_back(fieldTypeInspector);
}

void RecordInspector::setIntegerPrimaryKeyFieldIndex(int integerPrimaryKeyFieldIndex) noexcept
{
    this->integerPrimaryKeyFieldIndex = integerPrimaryKeyFieldIndex;
}

/**
 * ���ҳ��ָ��ƫ��λ�ÿ�ʼ�������Ƿ���ϼ�¼�����ֶ����Ͷ����������
 * @param page ҳ��������ʼָ�룻
 * @param offset ��ʼ���λ�ã�ҳ��ƫ�ƣ���
 * @param varints �ӿ�ʼ���λ����ȡ��(�ֶ�����-1)���䳤����������Щ�䳤������������ݿ���2���ֶε����ͣ�
 * @param varintsBytes varintsռ�õ��ֽ�������
 * @param incident �¹ʣ�
 * @return �Ƿ���ϼ�¼�ֶ�����������
 */

bool RecordInspector::inspect(char* page, uint32_t offset, vector<int64_t>& varints, uint32_t& varintsBytes, Incident& incident) const noexcept
{
    if(offset >= this->pageLength)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "Page offset[" + to_string(offset)+ "] overflow to pageLength[" + to_string(this->pageLength) + "]!", __FILE__, __func__, __LINE__);
        return false;
    }

    // ����2���ֶεı��޷�������
    if(this->fieldTypeInspectors.size() < 2)
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "FieldNum[" + to_string(this->fieldTypeInspectors.size())+ "] < 2, cannot inspect!", __FILE__, __func__, __LINE__);
        return false;
    }

    // �ӵ�ǰλ�ý�����(�ֶ�����-1)���䳤������
    varints = SqliteFacility::parseVarints(page + offset, this->pageLength, (uint32_t)this->fieldTypeInspectors.size()-1, varintsBytes, incident);
    if(!incident.succeeded())
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "", __FILE__, __func__, __LINE__);
        return false;
    }

    // Inspect: Fields
    for(uint32_t fieldIndex = 1; fieldIndex < this->fieldTypeInspectors.size(); fieldIndex ++)
    {
        bool isConformed = this->fieldTypeInspectors[fieldIndex].inspect(varints[fieldIndex-1]);
        if(!isConformed) return false;
    }
    return true;
}

// �Ʋ�Table B-Tree��¼�ĵ�һ���ֶ�����ռ�õ��ֽ�������
static uint8_t speculateFirstTypeBytes(Field const& firstField, const RecordSkippedHead* recordSkippedHead) noexcept
{
    if((firstField.getTypeAffinity() == Field::TypeAffinity::SQLITE_INTEGER) || (firstField.getTypeAffinity() == Field::TypeAffinity::SQLITE_REAL))
    {
        return 1;
    }
    else
    {
        int64_t skippedFirstFieldType = -1;
        if((recordSkippedHead != nullptr) && recordSkippedHead->isValid(RecordSkippedHead::SKIPPEDHEAD_FIRSTFIELD_TYPE)) skippedFirstFieldType = recordSkippedHead->getFirstFieldType();
        if((firstField.getTypeAffinity() == Field::TypeAffinity::SQLITE_TEXT) && SqliteFacility::isFieldTypeText(skippedFirstFieldType))
        {
            return recordSkippedHead->getFirstFieldTypeBytes();
        }
        else if((firstField.getTypeAffinity() == Field::TypeAffinity::SQLITE_BLOB) && SqliteFacility::isFieldTypeBlob(skippedFirstFieldType))
        {
            return recordSkippedHead->getFirstFieldTypeBytes();
        }
        return 1;
    }
}

// �Ʋ�Table B-Tree��¼�ĵ�һ���ֶ�����ռ�õ��ֽ����������͡�ֵռ���ֽ������ֵ�������Ʋ�����λ��־��
static const int SPECULATION_VALID_FIRSTTYPE_BYTES  = 0x0001;
static const int SPECULATION_VALID_FIRSTTYPE        = 0x0002;
static const int SPECULATION_VALID_FIRSTVALUE_BYTES = 0x0004;
static const int SPECULATION_VALID_FIRSTVALUE       = 0x0008;
int RecordInspector::speculateFirstFieldTypeAndValue(uint32_t pageNumber, char* page, const RecordSkippedHead* recordSkippedHead, uint32_t dataOffset,
        uint32_t& firstTypeBytes, int64_t& firstType, uint32_t& firstValueBytes, shared_ptr<FieldValue>& firstValue, Incident& incident) const noexcept
{
    // ��飻
    if(dataOffset >= this->pageLength)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "dataOffset[" + to_string(dataOffset) + "] overflow to pageLength[" + to_string(this->pageLength) + "]!", __FILE__, __func__, __LINE__);
        return 0;
    }

    // �Ʋ��һ�������ֽ���;
    int validFlags = 0;
    Field firstField = this->table->getFieldByFieldIndex(0);
    firstTypeBytes = speculateFirstTypeBytes(firstField, recordSkippedHead);
    validFlags |= SPECULATION_VALID_FIRSTTYPE_BYTES;

    int64_t skippedFirstFieldType = -1;
    if((recordSkippedHead != nullptr) && recordSkippedHead->isValid(RecordSkippedHead::SKIPPEDHEAD_FIRSTFIELD_TYPE)) skippedFirstFieldType = recordSkippedHead->getFirstFieldType();

    // Null;
    if(skippedFirstFieldType == 0)
    {
        firstType = 0;
        firstValueBytes = 0;
        if((firstField.getTypeAffinity() == Field::TypeAffinity::SQLITE_INTEGER) && (this->integerPrimaryKeyFieldIndex == 0) && (recordSkippedHead != nullptr) && recordSkippedHead->isValid(RecordSkippedHead::SKIPPEDHEAD_FIRSTFIELD_TYPE | RecordSkippedHead::SKIPPEDHEAD_ROWID))
        {
            firstValue = shared_ptr<FieldValue>(new FieldValue(pageNumber, 0, firstField.getFieldName(), 0, dataOffset, 0, recordSkippedHead->getRowId()));
        }
        else
        {
            firstValue = shared_ptr<FieldValue>(new FieldValue(pageNumber, 0, firstField.getFieldName()));
        }
        validFlags |= (SPECULATION_VALID_FIRSTTYPE | SPECULATION_VALID_FIRSTVALUE_BYTES | SPECULATION_VALID_FIRSTVALUE);
        incident.reset();
        return validFlags;
    }

    // Integer;
    if((firstField.getTypeAffinity() == Field::TypeAffinity::SQLITE_INTEGER) && SqliteFacility::isFieldTypeInteger(skippedFirstFieldType))
    {
        firstType = skippedFirstFieldType;
        int64_t integerValue = SqliteFacility::parseIntegerValue(page + dataOffset, (uint32_t)(this->pageLength - dataOffset), firstType, firstValueBytes, incident);
        if(incident.succeeded())
        {
            firstValue = shared_ptr<FieldValue>(new FieldValue(pageNumber, 0, firstField.getFieldName(), firstType, dataOffset, firstValueBytes, integerValue));
            validFlags |= (SPECULATION_VALID_FIRSTTYPE | SPECULATION_VALID_FIRSTVALUE_BYTES | SPECULATION_VALID_FIRSTVALUE);
        }
        incident.reset();
        return validFlags;
    }

    // Real;
    if(firstField.getTypeAffinity() == Field::TypeAffinity::SQLITE_REAL)
    {
        firstType = 7;
        validFlags |= SPECULATION_VALID_FIRSTTYPE;
        double realValue = SqliteFacility::parseRealValue(page + dataOffset, this->pageLength - dataOffset, firstValueBytes, incident);
        if(incident.succeeded())
        {
            firstValue = shared_ptr<FieldValue>(new FieldValue(pageNumber, 0, firstField.getFieldName(), firstType, dataOffset, firstValueBytes, realValue));
            validFlags |= (SPECULATION_VALID_FIRSTVALUE_BYTES | SPECULATION_VALID_FIRSTVALUE);
        }
        incident.reset();
        return validFlags;
    }

    // Text;
    if((firstField.getTypeAffinity() == Field::TypeAffinity::SQLITE_TEXT) && SqliteFacility::isFieldTypeText(skippedFirstFieldType))
    {
        bool partial;
        firstType = skippedFirstFieldType;
        string textValue = SqliteFacility::parseTextValue(page + dataOffset, this->pageLength - dataOffset, firstType, false, partial, firstValueBytes, incident);
        if(incident.succeeded())
        {
            firstValue = shared_ptr<FieldValue>(new FieldValue(pageNumber, 0, firstField.getFieldName(), firstType, dataOffset, firstValueBytes, textValue, false));
            validFlags |= (SPECULATION_VALID_FIRSTTYPE | SPECULATION_VALID_FIRSTVALUE_BYTES | SPECULATION_VALID_FIRSTVALUE);
        }
        incident.reset();
        return validFlags;
    }

    // Blob;
    if((firstField.getTypeAffinity() == Field::TypeAffinity::SQLITE_BLOB) && SqliteFacility::isFieldTypeBlob(skippedFirstFieldType))
    {
        bool partial;
        firstType = skippedFirstFieldType;
        shared_ptr<char> blobValue = SqliteFacility::parseBlobValue(page + dataOffset, this->pageLength - dataOffset, firstType, false, partial, firstValueBytes, incident);
        if(incident.succeeded())
        {
            firstValue = shared_ptr<FieldValue>(new FieldValue(pageNumber, 0, firstField.getFieldName(), firstType, dataOffset, firstValueBytes, blobValue, false));
            validFlags |= (SPECULATION_VALID_FIRSTTYPE | SPECULATION_VALID_FIRSTVALUE_BYTES | SPECULATION_VALID_FIRSTVALUE);
        }
        incident.reset();
        return validFlags;
    }
    incident.reset();
    return validFlags;
}

// �����ֶ����ͣ������Ӧ��Щ�ֶε�ֵռ�õ��ֽ�������
static uint32_t calculateValueBytesByFieldTypes(vector<int64_t> const& fieldTypes, Incident& incident) noexcept
{
    uint32_t valuesBytes = 0;
    for(int64_t fieldType: fieldTypes)
    {
        if(fieldType < 0)
        {
            incident.set(SqliteReturnCode::FAIL_GENERAL, "fieldType[" + to_string(fieldType) + "] < 0!", __FILE__, __func__, __LINE__);
            return 0;
        }
        else if((fieldType == 0) || (fieldType == 8) || (fieldType == 9)) continue;
        else if(fieldType == 1) valuesBytes ++;
        else if(fieldType == 2) valuesBytes += 2;
        else if(fieldType == 3) valuesBytes += 3;
        else if(fieldType == 4) valuesBytes += 4;
        else if(fieldType == 5) valuesBytes += 6;
        else if((fieldType == 6) || (fieldType == 7)) valuesBytes += 8;
        else if((fieldType == 10) || (fieldType == 11))
        {
            incident.set(SqliteReturnCode::FAIL_GENERAL, "fieldType[" + to_string(fieldType) + "] unused!", __FILE__, __func__, __LINE__);
            return 0;
        }
        else if(fieldType >= 0x7fffffff)
        {
            incident.set(SqliteReturnCode::FAIL_GENERAL, "fieldType[" + to_string(fieldType) + "] too long!", __FILE__, __func__, __LINE__);
            return 0;
        }
        else
        {
            valuesBytes += ((uint32_t)(fieldType - 12) >> 1);
        }
    }

    incident.reset();
    return valuesBytes;
}

// ���㳬����¼���ش�С��ע�⣺����֮ǰ��Բ����������ȼ��: (recordSize > this->maxLocalSize) && (maxLocalSize > minLocalSize > 0)��
static void calculateLocalSizeAndOverflowSizeOfTBTRecord(uint32_t usableSize, uint32_t minLocalSize, uint32_t maxLocalSize, uint32_t recordSize, uint32_t& localSize, uint32_t& overflowSize) noexcept
{
    localSize = minLocalSize + (recordSize - minLocalSize) % (usableSize - 4);
    if(localSize > maxLocalSize) localSize = minLocalSize;

    overflowSize = recordSize - localSize;
}

/**
 * ���������¼���ز��֣���'��ʼ�����ҳ��ƫ��λ��'�Ļ����ϣ�����ҳβ����ԭ���Լ�ҳ���Ų��ܳ�����Χ��ԭ�򣬰���һ������˳��������΢����
 * ����������ܵ�ҳ��λ�Ƽ���ҳ�ż��ϣ��Թ����������д���;
 * @param page ҳ�����ݣ�
 * @param totalPages ���ݿ���ҳ����
 * @param pageLength ҳ�泤�ȣ���ҳ���С��ȥβ�������ռ䳤�ȣ�
 * @param originalOfpno ��ʼ�����ҳ��ƫ��λ�ã����ڵ��øú���֮ǰ��ͨ���ض������жϵļ�¼��һ�����ҳ�ŵ�ƫ��λ�ã�
 * @param ofpnoToFofpns ��OUT��<΢��������ҳ��ƫ��λ��>��Ӧ<��һ�����ҳ��>��������
 * @param incident �¹ʣ�
 */
static void serveyLocalPartOfOverflowedRecord(char* page, uint32_t totalPages, uint32_t pageLength, uint32_t originalOfpno, vector<pair<uint32_t, uint32_t>>& ofpnoToFofpns, Incident& incident) noexcept
{
    if(originalOfpno <= 10)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "overflowPageNumberOffset[" + to_string(originalOfpno) + "] <= 10!", __FILE__, __func__, __LINE__);
        return;
    }

    // ����ҳβ����ԭ��������ز��ִ�С��
    int32_t d = originalOfpno + 4 - pageLength;
    if((d <= 3) && (d >= -3))
    {
        uint32_t ofpno = originalOfpno - d;

        uint32_t ofpn = CommonFacility::htobe32(*((uint32_t*)(page + ofpno)));
        if((ofpn > 1) && (ofpn <= totalPages))
        {
            ofpnoToFofpns.push_back(pair<uint32_t, uint32_t>(ofpno, ofpn));
        }
    }
    // �������ҳ��Χ���е�����
    else if(d < 0)
    {
        const static int deltas[] = {0, 1, 2, -1, 3, -2, -3};
        for(uint32_t i = 0; i < 7; i ++)
        {
            int delta = deltas[i];
            uint32_t ofpno = originalOfpno + delta;
            uint32_t ofpn = CommonFacility::htobe32(*((uint32_t*)(page + ofpno)));
            if((ofpn > 1) && (ofpn <= totalPages))
            {
                ofpnoToFofpns.push_back(pair<uint32_t, uint32_t>(ofpno, ofpn));
            }
        }
    }
    else
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "overflowPageNumberOffset[" + to_string(originalOfpno) + "] + 4 > pageLength[" + to_string(pageLength) + "]!", __FILE__, __func__, __LINE__);
        return;
    }
}

/**
 * �������¼�ĵ�һ�����ҳ�ſ�ʼ��׷�ټ�¼�����ҳ������ֱ��ҳ����Ч�������ҳ�ռ����Գ����������Ϊֹ��
 * @param database ���ݿ����ָ�룻
 * @param totalPages ���ݿ���ҳ����
 * @param pageSize ҳ���С��
 * @param pageLength ҳ�泤�ȣ���ҳ���С��ȥβ�������ռ䳤�ȣ�
 * @param overflowSize ��¼������ִ�С��
 * @param firstOfpn ��һ�����ҳ�ţ�
 * @param ofpnChain ��OUT����¼�����ҳ������
 */
static void traceOverflowedPagesOfRecord(Database* database, uint32_t totalPages, uint32_t pageSize, uint32_t pageLength, uint32_t overflowSize, uint32_t firstOfpn,
                                 vector<uint32_t>& ofpnChain) noexcept
{
    // ��ȡ�������ݿ����ݣ�
    Incident incident;
    const char* databaseData = (database->getData(incident)).get();

    // ��ʼ���٣�
    uint32_t ofpn = firstOfpn;
    uint32_t ofs = 0;
    while((ofpn > 1) && (ofpn <= totalPages))
    {
        ofpnChain.push_back(ofpn);
        ofs += pageLength;
        if(ofs + 3 > overflowSize) break;

        const char* page = databaseData + (ofpn - 1) * pageSize;
        ofpn = CommonFacility::htobe32(*((uint32_t*)page));
    }
}

/**
 * ���������¼������һϵ���Ż��͵����������������Ż��ļ�¼���ش�С�������С�����ҳ������
 * @param database ���ݿ����ָ�룻
 * @param page ҳ�����ݣ�
 * @param totalPages ���ݿ���ҳ����
 * @param pageSize ҳ���С��
 * @param pageLength ҳ�泤�ȣ���ҳ���С��ȥβ�������ռ䳤�ȣ�
 * @param headerSize ��¼ͷ��С��
 * @param dataOffset ��¼ֵƫ�ƣ�
 * @param localSize ��¼���ش�С��
 * @param overflowSize ��¼������ִ�С��
 * @param tunedLocalSize (OUT)΢����ļ�¼���ش�С��
 * @param tunedOverflowSize (OUT)΢����ļ�¼������ִ�С��
 * @param overflowPageNumberChain (OUT)΢����ļ�¼���ҳ������
 * @param incident �¹ʣ�
 */
static void serveyOverflowedRecord(Database* database, char* page, uint32_t totalPages, uint32_t pageSize, uint32_t pageLength, uint32_t headerSize, uint32_t dataOffset, uint32_t localSize, uint32_t overflowSize,
                                   uint32_t& tunedLocalSize, uint32_t& tunedOverflowSize, vector<uint32_t>& overflowPageNumberChain, Incident& incident) noexcept
{
    // ��鱾�ز��֣�
    if((localSize <= headerSize) || (localSize <= 3))
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "localSize[" + to_string(localSize) + "] <= headerSize[" + to_string(headerSize) + "] or 3!", __FILE__, __func__, __LINE__);
        return;
    }

    // ���ҳ���ƫ�ƣ�
    const uint32_t originalOfpno = dataOffset + localSize - headerSize;

    // ���챾�ز���(OverflowPageNumberOffset->FirstOverflowPageNumber)��
    vector<pair<uint32_t, uint32_t>> ofpnoToFofpns;
    serveyLocalPartOfOverflowedRecord(page, totalPages, pageLength, originalOfpno, ofpnoToFofpns, incident);
    if(!incident.succeeded()) return;

    // ����&��ѡ���ҳ����(�����Ϊ����)��
    uint32_t overflowPageNumberSize = 0;
    uint32_t overflowPageNumberOffset;
    overflowPageNumberChain.clear();
    for(auto ofpnoToFofpn: ofpnoToFofpns)
    {
        uint32_t ofpno = ofpnoToFofpn.first;
        uint32_t fofpn = ofpnoToFofpn.second;

        vector<uint32_t> ofpnChain;
        traceOverflowedPagesOfRecord(database, totalPages, pageSize, pageLength, overflowSize, fofpn, ofpnChain);

        if(ofpnChain.size() > overflowPageNumberSize)
        {
            overflowPageNumberSize = (uint32_t)ofpnChain.size();
            overflowPageNumberOffset = ofpno;
            overflowPageNumberChain = ofpnChain;
        }
    }

    // �����
    if(overflowPageNumberSize == 0)
    {
        tunedLocalSize = localSize;
        tunedOverflowSize = overflowSize;
    }
    else
    {
        tunedLocalSize = overflowPageNumberOffset - dataOffset + headerSize;
        tunedOverflowSize = localSize + overflowSize - tunedLocalSize;
    }
}

// �ӵڶ����ֶο�ʼ������¼���ֶ�ֵ��
static uint32_t parseFieldValuesFromSecondField(Table const& table, uint32_t pageNumber, char* data, uint32_t dataLength, bool processOverflowRecord, vector<int64_t> const& fieldsTypes,
        uint32_t& valuesBytes, shared_ptr<Record>& record, Incident& incident) noexcept
{
    valuesBytes = 0;
    uint32_t valueOffset = 0;
    uint32_t fieldNum = 0;

    incident.reset();
    for(uint32_t fieldIndex = 1; fieldIndex <= fieldsTypes.size(); fieldIndex ++)
    {
        int64_t fieldType = fieldsTypes[fieldIndex - 1];
        const Field& field = table.getFieldByFieldIndex(fieldIndex);

        uint32_t valueBytes = 0;
        bool partial;
        shared_ptr<FieldValue> fieldValue = FieldValue::parse(data, dataLength, valueOffset, pageNumber, fieldIndex, field.getFieldName(), fieldType, processOverflowRecord, partial, valueBytes, incident);
        if(!incident.succeeded()) break;
        record->add(fieldValue);
        valuesBytes += valueBytes;
        valueOffset += valueBytes;
        fieldNum ++;
    }

    if(processOverflowRecord && (incident.getCode() == SqliteReturnCode::FAIL_OVERFLOW))
    {
        incident.reset();
    }
    return fieldNum;
}

void RecordInspector::inspectFieldValues(uint32_t pageNumber, char* page, vector<int64_t> const& varints, uint32_t varintsOffset, uint32_t const& varintsBytes,
        shared_ptr<Record>& record, uint32_t& recordValueBytes, Incident& incident) const noexcept
{
    // �趨����ҳ�ţ�
    record->clear();
    record->setPageNumber(pageNumber);

    // ������¼��ȥ��ͷ����
    shared_ptr<RecordSkippedHead> recordSkippedHead = RecordSkippedHead::parse(page, varintsOffset);
    record->setRecordSkippedHead(recordSkippedHead);

    // ��¼�ֶ�ֵ��ʼƫ�ƣ�
    const uint32_t dataOffset = varintsOffset + varintsBytes;

    // �Ʋ��һ���ֶε�������ݣ�
    uint32_t firstTypeBytes;
    int64_t firstType;
    uint32_t firstValueBytes;
    shared_ptr<FieldValue> firstValue;
    int validFlags = this->speculateFirstFieldTypeAndValue(pageNumber, page, recordSkippedHead.get(), dataOffset, firstTypeBytes, firstType, firstValueBytes, firstValue, incident);
    if(!incident.succeeded())
    {
        incident.set(incident.getCode(), incident.getMessage(), __FILE__, __func__, __LINE__);
        return;
    }

    // �Ʋ�ĵ�һ���ֶ�ֵ��
    if((validFlags & SPECULATION_VALID_FIRSTVALUE) == SPECULATION_VALID_FIRSTVALUE) record->add(firstValue);

    // �Ʋ��¼ͷ��С��Header Size����
    if((validFlags & SPECULATION_VALID_FIRSTTYPE_BYTES) != SPECULATION_VALID_FIRSTTYPE_BYTES) firstTypeBytes = 1;
    int64_t headerSize = varintsBytes + firstTypeBytes + 1;
    if(headerSize > 127) headerSize ++;

    // �Ʋ��¼��С��Record Size����
    if((validFlags & SPECULATION_VALID_FIRSTVALUE_BYTES) != SPECULATION_VALID_FIRSTVALUE_BYTES) firstValueBytes = 0;
    uint32_t valuesBytesFromSecond = calculateValueBytesByFieldTypes(varints, incident);
    if(!incident.succeeded())
    {
        incident.set(incident.getCode(), incident.getMessage(), __FILE__, __func__, __LINE__);
        return;
    }
    int64_t recordSize = headerSize + firstValueBytes + valuesBytesFromSecond;

    // ������¼��С�������¼��С���һص���ȥͷ���ļ�¼��С�����3���ֽ����ڣ�������ȥͷ���ļ�¼��СΪ׼����
    if((recordSkippedHead != nullptr) && recordSkippedHead->isValid(RecordSkippedHead::SKIPPEDHEAD_RECORD_SIZE))
    {
        int64_t skippedRecordSize = recordSkippedHead->getRecordSize();
        if(((skippedRecordSize ^ recordSize) & -4) == 0) recordSize = skippedRecordSize;
    }

    // ��������¼�Ƿ������
    if(this->processOverflowRecord && (recordSize > this->maxLocalSize) && (this->maxLocalSize > 0))
    {
        // �����¼�ı��س��Ⱥ�������ȣ�
        uint32_t localSize;
        uint32_t overflowSize;
        calculateLocalSizeAndOverflowSizeOfTBTRecord(this->pageLength, this->minLocalSize, this->maxLocalSize, (uint32_t)recordSize, localSize, overflowSize);

		// ���� & �Ż�������ݣ�
        uint32_t tunedLocalSize = localSize;
        uint32_t tunedOverflowSize = overflowSize;
        vector<uint32_t> overflowPageNumberChain;
        serveyOverflowedRecord(this->database, page, this->totalPages, this->pageSize, this->pageLength, (uint32_t)headerSize, dataOffset, localSize, overflowSize,
                               tunedLocalSize, tunedOverflowSize, overflowPageNumberChain, incident);
        if(!incident.succeeded()) return;

        // ȡֵ��
        const uint32_t tunedRecordSize = tunedLocalSize + tunedOverflowSize;
        const uint32_t dataSize = (uint32_t)(tunedRecordSize - headerSize);
        if((overflowPageNumberChain.size() == 0) || (dataSize <= tunedLocalSize) || (dataSize > 8 * 1024 * 1024))
        {
            // �ӵڶ����ֶν�����¼ֵ��
            uint32_t recordValueBytesFromSecondField;
            uint32_t secondFieldValueOffset = dataOffset + firstValueBytes;
            parseFieldValuesFromSecondField(*(this->table), pageNumber, page + secondFieldValueOffset, this->pageLength - secondFieldValueOffset, this->processOverflowRecord, varints, recordValueBytesFromSecondField, record, incident);
            recordValueBytes = firstValueBytes + recordValueBytesFromSecondField;
        }
        else
        {
            // ��飻
            if(tunedLocalSize <= headerSize + firstValueBytes)
            {
                incident.set(SqliteReturnCode::FAIL_OVERFLOW, "tunedLocalSize[" + to_string(tunedLocalSize) + "] <= headerSize[" + to_string(headerSize) + "] + firstValueBytes[" + to_string(firstValueBytes) + "]!", __FILE__, __func__, __LINE__);
                return;
            }

            // ��ȡ���ݿ�����;
            const char* databaseData = database->getData(incident).get();

            // ��֯��¼������ֵ����;
            shared_ptr<char> wholeRecordDataForAutoDelete(new char[dataSize], default_delete<char[]>());
            char* wholeRecordData = wholeRecordDataForAutoDelete.get();
            uint32_t realRecordDataSize = 0;

            // ���Ʊ������ݣ�
            const uint32_t localDataSize = (uint32_t)(tunedLocalSize - headerSize);
            memcpy(wholeRecordData, page + dataOffset, localDataSize);
            realRecordDataSize += localDataSize;

            // ������ݣ�
            uint32_t remindarRecordDataSize = dataSize - realRecordDataSize;
            for(uint32_t& overflowPageNumber: overflowPageNumberChain)
            {
                // ���ҳ���ݣ�
                const char* overflowPage = databaseData + (overflowPageNumber - 1) * this->pageSize;

                // ���ҳ���ݳ��ȣ�
                uint32_t k = ((remindarRecordDataSize <= (this->pageLength - 4))?remindarRecordDataSize:(this->pageLength - 4));

                // �������ҳ���ݣ�
                memcpy(wholeRecordData + realRecordDataSize, overflowPage + 4, k);

                realRecordDataSize += k;
                remindarRecordDataSize -= k;
                if(k == 0) break;
            }

            // �ӵڶ����ֶν�����¼ֵ��
            uint32_t recordValueBytesFromSecondField;
            parseFieldValuesFromSecondField(*(this->table), pageNumber, wholeRecordData + firstValueBytes, realRecordDataSize - firstValueBytes, true, varints, recordValueBytesFromSecondField, record, incident);
            recordValueBytes = firstValueBytes + recordValueBytesFromSecondField;
        }
    }
    else
    {
        // �ӵڶ����ֶν�����¼ֵ��
        uint32_t recordValueBytesFromSecondField;
        uint32_t secondFieldValueOffset = dataOffset + firstValueBytes;
        parseFieldValuesFromSecondField(*(this->table), pageNumber, page + secondFieldValueOffset, this->pageLength - secondFieldValueOffset, true, varints, recordValueBytesFromSecondField, record, incident);
        recordValueBytes = firstValueBytes + recordValueBytesFromSecondField;
    }

    return;
}

shared_ptr<RecordInspector> RecordInspector::deduce(Database& database, Table const& table, bool processOverflowRecord) noexcept
{
    // ����Ƿ�Ϊ����������
    vector<Field> primaryKey = table.getPrimarykey();
    bool hasIntegerPrimaryKey = ((primaryKey.size() == 1) && (primaryKey[0].getTypeAffinity() == Field::TypeAffinity::SQLITE_INTEGER));

    // ���ݱ��ÿ��������ͣ�����������������
    shared_ptr<RecordInspector> recordInspector(new RecordInspector(database, table, processOverflowRecord));
    for(unsigned int i = 0; i < table.getFieldNum(); i ++)
    {
        const Field& field = table.getFieldByFieldIndex(i);

        FieldTypeInspector fieldTypeInspector(field.getTypeAffinity());

        // Nullable;
        if(!field.isNotNull()) fieldTypeInspector.setNullable();

        // �������������
        if(hasIntegerPrimaryKey && (primaryKey[0].getFieldName() == field.getFieldName()))
        {
            hasIntegerPrimaryKey = false;
            fieldTypeInspector.setMinValue(0);
            fieldTypeInspector.setMaxValue(0);
            recordInspector->setIntegerPrimaryKeyFieldIndex(i);
        }

        recordInspector->pushBack(fieldTypeInspector);
    }

    return recordInspector;
}

ostream& hudun::sqlite::operator<<(ostream& out, RecordInspector const& recordInspector) noexcept
{
    // pageLength;
    out << "Page Length: " << recordInspector.pageLength << endl;

    // fieldTypeInspectors;
    for(uint32_t i = 0; i < recordInspector.fieldTypeInspectors.size(); i ++)
    {
        out << recordInspector.fieldTypeInspectors[i] << endl;
    }

    return out;
}

