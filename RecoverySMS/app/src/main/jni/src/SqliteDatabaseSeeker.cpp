#include <memory>
#include <thread>
using namespace std;

#include "../include/Incident.h"
#include "../include/Timer.h"
using namespace hudun::common;

#include "../include/Database.h"
#include "../include/RecordFilter.h"
#include "../include/SeekEventHandler.h"
#include "../include/SeekProgressIndicator.h"
#include "../include/SqliteBlockSeeker.h"
#include "../include/SqliteDatabaseSeeker.h"
using namespace hudun::sqlite;

#include "../include/Table.h"
using namespace hudun::sqlite::schema;

SqliteDatabaseSeeker::SqliteDatabaseSeeker(Database& database, hudun::sqlite::schema::Table const& table, RecordFilter* recordFilter, bool processOverflowRecord) noexcept
{
    this->database = &database;
    this->recordFilter = recordFilter;
    this->processOverflowRecord = processOverflowRecord;
    this->recordInspector = RecordInspector::deduce(database, table, processOverflowRecord);
}

SqliteDatabaseSeeker::SqliteDatabaseSeeker(SqliteDatabaseSeeker const& other) noexcept
{
    this->database = other.database;
    this->recordFilter = other.recordFilter;
    this->processOverflowRecord = other.processOverflowRecord;
    this->recordInspector = other.recordInspector;
}

SqliteDatabaseSeeker& SqliteDatabaseSeeker::operator=(SqliteDatabaseSeeker const& other) noexcept
{
    if(this != &other)
    {
        this->database = other.database;
        this->recordFilter = other.recordFilter;
        this->processOverflowRecord = other.processOverflowRecord;
        this->recordInspector = other.recordInspector;
    }
    return *this;
}

SqliteDatabaseSeeker::~SqliteDatabaseSeeker()
{
}

uint32_t SqliteDatabaseSeeker::seek(uint32_t threadNum, SeekProgressIndicator* seekProgressIndicator, SeekEventHandler* seekEventHandler, hudun::common::Incident& incident) noexcept
{
	SeekEventHandler** seekEventHandlers = new SeekEventHandler*[threadNum];
    for(uint32_t i = 0; i < threadNum; i ++)
    {
        seekEventHandlers[i] = seekEventHandler;
    }
    uint32_t seekedRecordNum = this->seek(threadNum, seekProgressIndicator, seekEventHandlers, incident);
	delete[] seekEventHandlers;
	return seekedRecordNum;
}

static void excuteSeekByThread(SqliteBlockSeeker* sqliteBlockSeeker, char* pageBlock, uint32_t pageNumberStart, uint32_t pageNum, SeekProgressIndicator* seekProgressIndicator, SeekEventHandler* seekEventHandler, uint32_t* seekedRecordNum) noexcept
{
    *seekedRecordNum = sqliteBlockSeeker->seek(pageBlock, pageNumberStart, pageNum, seekProgressIndicator, seekEventHandler);
}

uint32_t SqliteDatabaseSeeker::seek(uint32_t threadNum, SeekProgressIndicator* seekProgressIndicator, SeekEventHandler** seekEventHandlers, Incident& incident) noexcept
{
    const uint32_t totalPages = this->database->getPages();
    const uint32_t pageSize = this->database->getPageSize();
    const uint32_t pageLength = pageSize - this->database->getReservedSpacePerPage();

    // ����������ݣ�
    shared_ptr<char> databaseData = this->database->getData(incident);
    if(!incident.succeeded()) return 0;

    // ��������������
    uint32_t seekedRecordNum = 0;
    SqliteBlockSeeker sqliteBlockSeeker(pageSize, pageLength, this->recordInspector.get(), this->recordFilter);

    // ����ָʾ����λ��
    if(seekProgressIndicator != nullptr) seekProgressIndicator->init(totalPages);

    // ���߳�
    if((threadNum < 1) || (threadNum >= totalPages)) threadNum = 1;
    if(threadNum == 1)
    {
        seekedRecordNum = sqliteBlockSeeker.seek(databaseData.get(), 1, totalPages, seekProgressIndicator, seekEventHandlers[0]);
    }
    // ���̣߳�
    else
    {
        char** pageBlocks = new char*[threadNum];
        uint32_t* pageNumberStarts = new uint32_t[threadNum];
        uint32_t* pageNums = new uint32_t[threadNum];
        uint32_t* seekedRecordNums = new uint32_t[threadNum];
        thread* threads = new thread[threadNum];

        uint32_t blockPages = (totalPages / threadNum);
        uint32_t remainderPages = (totalPages % threadNum);
        for(uint32_t i = 0; i < threadNum; i ++)
        {
            if(i == 0)
            {
                pageBlocks[0] = databaseData.get();
                pageNumberStarts[0] = 1;
                pageNums[0] = blockPages + remainderPages;
            }
            else
            {
                pageBlocks[i] = pageBlocks[i-1] + pageNums[i-1] * pageSize;
                pageNumberStarts[i] = pageNumberStarts[i-1] + pageNums[i-1];
                pageNums[i] = blockPages ;
            }

            // �����̣߳�
            threads[i] = thread(excuteSeekByThread, &sqliteBlockSeeker, pageBlocks[i], pageNumberStarts[i], pageNums[i], seekProgressIndicator, seekEventHandlers[i], &(seekedRecordNums[i]));
        }

        // �ȴ��߳̽�����
        map<void*, uint32_t> hpToRecords;
        for(uint32_t i = 0; i < threadNum; i ++)
        {
            threads[i].join();

            // �������¼�������Ϊ��ͬʵ����ȡ�����ҵ���¼����
            auto hpIt = hpToRecords.find((void*)seekEventHandlers[i]);
            if(hpIt == hpToRecords.end())
            {
                hpToRecords.emplace((void*)seekEventHandlers[i], seekedRecordNums[i]);
            }
            else if(seekedRecordNums[i] > hpIt->second)
            {
                hpIt->second = seekedRecordNums[i];
            }
        }

        // ͳ���ҵ��ļ�¼������
        for(auto p: hpToRecords) seekedRecordNum += p.second;

		// �ͷ��ڴ棻
		delete[] pageBlocks;
		delete[] pageNumberStarts;
		delete[] pageNums;
		delete[] seekedRecordNums;
		delete[] threads;
    }

    if(seekProgressIndicator != nullptr) seekProgressIndicator->complete();
    return seekedRecordNum;
}
