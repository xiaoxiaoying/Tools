#include <algorithm>
#include <memory>
#include <set>
#include <string>
using namespace std;

#include "../include/CommonException.h"
#include "../include/CommonFacility.h"
#include "../include/Incident.h"
using namespace hudun::common;

#include "../include/Database.h"
#include "../include/Entity.h"
#include "internal/SqliteStructs.h"
#include "../include/SqliteFacility.h"
#include "../include/SqliteReturnCode.h"
#include "../include/TableBtree.h"
#include "../include/Strto.h"

using namespace hudun::sqlite;

TableBtree::TableBtree(uint16_t obtainWay) noexcept: Entity(obtainWay)
{
}

void TableBtree::setTblName(string const& tblName) noexcept
{
    this->tblName = tblName;
    this->validFlags |= FIELD_FLAG_TBL_NAME;
}

void TableBtree::setRootPage(uint32_t rootPage) noexcept
{
    this->rootPage = rootPage;
    this->validFlags |= FIELD_FLAG_ROOTPAGE;
}

string TableBtree::getTblName() const noexcept
{
    return this->tblName;
}

uint32_t TableBtree::getRootPage() const noexcept
{
    return this->rootPage;
}

const set<uint32_t>& TableBtree::getInteriorPageNumbers() const noexcept
{
    return this->interiorPageNumbers;
}

const set<uint32_t>& TableBtree::getLeafPageNumbers() const noexcept
{
    return this->leafPageNumbers;
}

static const char PAGE_FLAG_INTERIOR = '\x05';
static const char PAGE_FLAG_LEAF = '\x0d';

/**
 * ����TableBtree�ڲ�ҳ�����ݵ�Ԫ����ʽΪ��ChildPageNumber(uint32_t, big-endian), Key(varint, 1-9)
 *
 */
static void parseTableBtreeInteriorCell(char* buff, uint16_t buffLength, uint64_t& subPageNumber, int64_t& key, uint32_t& cellBytes, Incident& incident) noexcept
{
    // check buffLength;
    if(buffLength < 5)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "Parse table b-tree interior cell fail, because buffLength[" + to_string(buffLength) + "] < 5!", __FILE__, __func__, __LINE__);
        return;
    }

    // Page Number;
    subPageNumber = (uint32_t)SqliteFacility::parseUnsignedIntegerValue(buff, 4);

    // key;
    uint32_t varintBytes;
    incident.reset();
    key = SqliteFacility::parseVarint(buff + 4, buffLength - 4, varintBytes, incident);
    if(!incident.succeeded()) return;

    cellBytes = varintBytes + 4;
    return;
}

static void parseTableBtreeInteriorCells(char* buff, uint32_t buffLength, uint16_t cellNum, map<uint64_t, int64_t>& cells, uint32_t& cellsBytes, Incident& incident)
{
    cellsBytes = 0;
    uint32_t cellBytes = 0;
    uint64_t subPageNumber;
    int64_t key;
    for(uint32_t i = 0; i < cellNum; i ++)
    {
        parseTableBtreeInteriorCell(buff + cellsBytes, buffLength - cellsBytes, subPageNumber, key, cellBytes, incident);
        if(!incident.succeeded()) return;
        cells.emplace(subPageNumber, key);
        cellsBytes += cellBytes;
    }
}

typedef struct {
    shared_ptr<char> databaseData;
    uint32_t pages;
    uint32_t pageSize;
    uint32_t pageLength;
} NativeDatabaseInfo;

static char* getPageInDatabaseData(NativeDatabaseInfo const& ndi, uint32_t pageNumber, Incident& incident) noexcept
{
    if(pageNumber == 0)
    {
        incident.set(Incident::FAIL_GENERAL, "pageNumber is 0!", __FILE__, __func__, __LINE__);
        return nullptr;
    }

    if(pageNumber > ndi.pages)
    {
        incident.set(Incident::FAIL_GENERAL, "pageNumber[" + to_string(pageNumber) + "] overflow!", __FILE__, __func__, __LINE__);
        return nullptr;
    }
    incident.reset();
    return (ndi.databaseData.get() + (pageNumber - 1) * ndi.pageSize);
}

static void processPage(NativeDatabaseInfo const& ndi, uint32_t pageNumber, char* page, set<uint32_t>& interiorPageNumbers, set<uint32_t>& leafPageNumbers, Incident& incident) noexcept;
static void processInteriorPage(NativeDatabaseInfo const& ndi, uint32_t pageNumber, char* page, set<uint32_t>& interiorPageNumbers, set<uint32_t>& leafPageNumbers, Incident& incident) noexcept
{
    // ��ȡҳͷ��
    uint32_t headerOffet = ((pageNumber == 1)?100:0);
    StructPageBtreeHeader* spbh = (StructPageBtreeHeader*)(page + headerOffet);

    // �ڲ�ҳ��Ԫ������
    uint16_t cellNum = CommonFacility::be16toh(spbh->cells);

    // �ڲ�ҳ������ƫ�ƣ�
    uint16_t contentOffset = CommonFacility::be16toh(spbh->contentOffset);

    // �ڲ�ҳ����ҳ��
    uint32_t rightmostPageNumber = CommonFacility::be32toh(spbh->rightmostPageNumber);

    // ����������Ƿ�ƫ��Խ�磻
    if(contentOffset >= ndi.pageLength)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "cellContentOffset[" + to_string(contentOffset) + "] over pageLength[" + to_string(ndi.pageLength) + "]!", __FILE__, __func__, __LINE__);
        return;
    }

    // ������������ҳ�浥Ԫ��
    incident.reset();
    uint32_t cellsBytes;
    map<uint64_t, int64_t> cells;
    parseTableBtreeInteriorCells(page + contentOffset, ndi.pageLength - contentOffset, cellNum, cells, cellsBytes, incident);
    if(!incident.succeeded()) return;

    // ��������������ҳ��
    for(map<uint64_t, int64_t>::const_iterator it = cells.begin(); it != cells.end(); ++ it)
    {
        page = getPageInDatabaseData(ndi, (uint32_t)it->first, incident);
        if(!incident.succeeded()) continue;

        processPage(ndi, (uint32_t)it->first, page, interiorPageNumbers, leafPageNumbers, incident);
        if(!incident.succeeded()) return;
    }

    // �������ұߵ���ҳ��
    page = getPageInDatabaseData(ndi, rightmostPageNumber, incident);
    if(!incident.succeeded()) return;

    processPage(ndi, rightmostPageNumber, page, interiorPageNumbers, leafPageNumbers, incident);
}

static void processPage(NativeDatabaseInfo const& ndi, uint32_t pageNumber, char* page, set<uint32_t>& interiorPageNumbers, set<uint32_t>& leafPageNumbers, Incident& incident) noexcept
{
    // ��ֹ�����ģ�������ֹ�ݹ��ι��ȣ�
    const static uint32_t MAX_NUM_INTERIOR_NODE = 100;
    if(interiorPageNumbers.size() > MAX_NUM_INTERIOR_NODE)
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "Interior node num[" + to_string(interiorPageNumbers.size()) + "] too many!", __FILE__, __func__, __LINE__);
        return;
    }

    // ��ֹѭ���ݹ飻
    set<uint32_t>::const_iterator it = interiorPageNumbers.find(pageNumber);
    if(it != interiorPageNumbers.end())
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "pageNumber[" + to_string(pageNumber) + "] already in interior node!", __FILE__, __func__, __LINE__);
        return;
    }

    // ���ҳ�����ͣ�
    uint32_t headerOffet = ((pageNumber == 1)?100:0);
    char pageFlag = page[headerOffet];
    if(pageFlag == PAGE_FLAG_LEAF)
    {
        leafPageNumbers.insert(pageNumber);
    }
    else if(pageFlag == PAGE_FLAG_INTERIOR)
    {
        interiorPageNumbers.insert(pageNumber);
        processInteriorPage(ndi, pageNumber, page, interiorPageNumbers, leafPageNumbers, incident);
    }
}

void TableBtree::collect(Database& database, Incident& incident) noexcept
{
    // ������ݿ������Ϣ��
    NativeDatabaseInfo ndi;
    ndi.pages = database.getPages();
    ndi.pageSize = database.getPageSize();
    ndi.pageLength = ndi.pageSize - database.getReservedSpacePerPage();
    ndi.databaseData = database.getData(incident);
    if(!incident.succeeded()) return;

    // ��ȡ��ҳ��
    char* page = getPageInDatabaseData(ndi, this->rootPage, incident);
    if(!incident.succeeded()) return;

    // ��ʼ�����ռ��ڲ�ҳ�ţ�Ҷҳ�ţ�
    processPage(ndi, this->rootPage, page, this->interiorPageNumbers, this->leafPageNumbers, incident);
}
