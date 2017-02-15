#include <memory>
using namespace std;

#include "../include/Incident.h"
#include "../include/CommonFacility.h"
using namespace hudun::common;

#include "../include/RecordFilter.h"
#include "../include/RecordInspector.h"
#include "../include/SeekEventHandler.h"
#include "../include/SeekProgressIndicator.h"
#include "../include/SqliteBlockSeeker.h"
#include "../include/SqliteReturnCode.h"
using namespace hudun::sqlite;

SqliteBlockSeeker::SqliteBlockSeeker(uint32_t pageSize, uint32_t pageLength, RecordInspector* recordInspector, RecordFilter* recordFilter) noexcept
{
    this->pageSize = pageSize;
    this->pageLength = pageLength;
    this->recordInspector = recordInspector;
    this->recordFilter = recordFilter;

}

SqliteBlockSeeker::SqliteBlockSeeker(SqliteBlockSeeker const& other) noexcept
{
    this->pageSize = other.pageSize;
    this->pageLength = other.pageLength;
    this->recordInspector = other.recordInspector;
    this->recordFilter = other.recordFilter;
}

SqliteBlockSeeker& SqliteBlockSeeker::operator=(SqliteBlockSeeker const& other) noexcept
{
    if(this != &other)
    {
        this->pageSize = other.pageSize;
        this->pageLength = other.pageLength;
        this->recordInspector = other.recordInspector;
        this->recordFilter = other.recordFilter;
    }
    return *this;
}

SqliteBlockSeeker::~SqliteBlockSeeker()
{
}

static uint32_t inspectPage(RecordInspector* recordInspector, RecordFilter* recordFilter, SeekEventHandler* seekEventHandler, char* page, uint32_t pageNumber, Incident& incident) noexcept
{
 //bool b = (pageNumber == 176);
    vector<int64_t> varints;
    uint32_t varintsOffset = 8;
    uint32_t varintsBytes;
    uint32_t fieldValuesBytes;
    uint32_t seekedRecordNum = 0;
    shared_ptr<Record> record(new Record());
    while(true)
    {
        varints.clear();
        bool isConformed = recordInspector->inspect(page, varintsOffset, varints, varintsBytes, incident);

        if(incident.succeeded())
        {
            if(isConformed)
            {
                record->clear();
                recordInspector->inspectFieldValues(pageNumber, page, varints, varintsOffset, varintsBytes, record, fieldValuesBytes, incident);
                if(incident.succeeded() && recordFilter->check(record))
                {
                    seekedRecordNum += seekEventHandler->seekedRecord(record);
                    varintsOffset ++;   //= varintsBytes; 2016-04-07
                }
                else
                {
                    varintsOffset ++;
                }
            }
            else
            {
                varintsOffset ++;
            }
        }
        else
        {
            if(incident.getCode() != SqliteReturnCode::FAIL_OVERFLOW)
            {
                return seekedRecordNum;
            }
            else
            {
                break;
            }
        }
    }
    return seekedRecordNum;
}

uint32_t SqliteBlockSeeker::seek(char* pageBlock, uint32_t pageNumberStart, uint32_t pageNum, SeekProgressIndicator* seekProgressIndicator, SeekEventHandler* seekEventHandler) noexcept
{
    char* page;
    uint32_t pageNumber;
    uint32_t seekedRecordNum = 0;
    Incident incident;
    uint32_t progressPages = 0;
    uint32_t progressRecords = 0;
    for(uint32_t i = 0; i < pageNum; i ++)
    {
        page = pageBlock + i * this->pageSize;
        pageNumber = pageNumberStart + i;

        // ����ҳ��;
        seekEventHandler->beforeSeekPage(pageNumber);
        uint32_t recordNumOnePage = inspectPage(this->recordInspector, this->recordFilter, seekEventHandler, page, pageNumber, incident);
        seekEventHandler->afterSeekPage(pageNumber);
        seekedRecordNum += recordNumOnePage;
        progressPages ++;
        progressRecords += recordNumOnePage;
        if(progressPages >= this->pagesPerProgress)
        {
            if(seekProgressIndicator != nullptr) seekProgressIndicator->inc(progressPages, progressRecords);
            progressPages = 0;
            progressRecords = 0;
        }
    }
    if(seekProgressIndicator != nullptr) seekProgressIndicator->inc(progressPages, progressRecords);
    return seekedRecordNum;
}
