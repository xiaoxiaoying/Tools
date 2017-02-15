#include <cstring>
#include <iostream>
#include <memory>
#include <string>
using namespace std;

#include "../include/CommonException.h"
#include "../include/CommonFacility.h"
#include "../include/Incident.h"
#include "../include/StringFacility.h"
using namespace hudun::common;

#include "../include/Database.h"
#include "../include/SqliteReturnCode.h"
#include "internal/SqliteStructs.h"
#include "../include/Strto.h"

using namespace hudun::sqlite;

Database::Database() noexcept
{
}

Database::~Database()
{
    this->close();
}

void Database::open(string const& dbFilename)
{
    Incident incident;
    this->open(dbFilename, incident);
    if(!incident.succeeded())
    {
        throw CommonFacility::translate(incident);
    }
}

void Database::open(string const& dbFilename, Incident& incident) noexcept
{
    // ���ļ���
    incident.reset();
    this->close();
    this->dbStream.open(dbFilename, ios::in | ios::binary);
    if(!this->dbStream.good())
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: open[" + dbFilename + "] fail!", __FILE__, __func__, __LINE__);
        return;
    }

    // �������ݿ�ͷ;
    StructDatabaseHeader structDatabaseHeader;
    this->dbStream.seekg (0, ios::beg);
    this->dbStream.read((char*)&structDatabaseHeader, 100);

    // ��飻
    // check: magic number;
    if(memcmp(structDatabaseHeader.magicNumber, "SQLite format 3", 16) != 0)
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: magic number invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: page size;
    uint16_t pageSize = CommonFacility::be16toh(structDatabaseHeader.pageSize);
    if(!((pageSize == 1) || (((pageSize / 512) >= 1) && ((pageSize % 512) == 0))))
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: page size[" + to_string(pageSize) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: fileFormatWriteVersion;
    if((structDatabaseHeader.fileFormatWriteVersion != 1) && (structDatabaseHeader.fileFormatWriteVersion != 2))
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: File format write version[" + to_string(structDatabaseHeader.fileFormatWriteVersion) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: fileFormatReadVersion;
    if((structDatabaseHeader.fileFormatReadVersion != 1) && (structDatabaseHeader.fileFormatReadVersion != 2))
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: File format read version[" + to_string(structDatabaseHeader.fileFormatReadVersion) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: reservedSpacePerPage;
    if(structDatabaseHeader.reservedSpacePerPage > 32)
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: Reserved space[" + to_string(structDatabaseHeader.reservedSpacePerPage) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: maxEmbeddedPayloadFraction;
    if(structDatabaseHeader.maxEmbeddedPayloadFraction != 64)
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: Maximum embedded payload fraction[" + to_string(structDatabaseHeader.maxEmbeddedPayloadFraction) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: minEmbeddedPayloadFraction;
    if(structDatabaseHeader.minEmbeddedPayloadFraction != 32)
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: Minimum  embedded payload fraction[" + to_string(structDatabaseHeader.minEmbeddedPayloadFraction) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: leafPayloadFraction;
    if(structDatabaseHeader.leafPayloadFraction != 32)
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: Leaf payload fraction[" + to_string(structDatabaseHeader.leafPayloadFraction) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // pages;
    uint32_t pages = CommonFacility::be32toh(structDatabaseHeader.pages);

    // check: firstFreelistPageNumber;
    uint32_t firstFreelistPageNumber = CommonFacility::be32toh(structDatabaseHeader.firstFreelistPageNumber);
    if(firstFreelistPageNumber > pages)
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: Page number of the first freelist trunk page[" + to_string(firstFreelistPageNumber) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: freelistPages;
    uint32_t freelistPages = CommonFacility::be32toh(structDatabaseHeader.freelistPages);
    if(freelistPages > pages)
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: Total number of freelist pages[" + to_string(freelistPages) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: schemaFormatNumber;
    uint32_t schemaFormatNumber = CommonFacility::be32toh(structDatabaseHeader.schemaFormatNumber);
    if((schemaFormatNumber < 1) || (schemaFormatNumber > 4))
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: The schema format number[" + to_string(schemaFormatNumber) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: largestRootBTreePageNumber;
    uint32_t largestRootBTreePageNumber = CommonFacility::be32toh(structDatabaseHeader.largestRootBTreePageNumber);
    if(largestRootBTreePageNumber > pages)
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: The page number of the largest root b-tree page[" + to_string(largestRootBTreePageNumber) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    // check: textEncoding;
    uint32_t textEncoding = CommonFacility::be32toh(structDatabaseHeader.textEncoding);
    if((textEncoding < 1) || (textEncoding > 3))
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: The database text encoding[" + to_string(textEncoding) + "] invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    if(textEncoding != TextEncoding::UTF_8)
    {
        incident.set(Incident::FAIL_GENERAL, "SqliteDB: The database text encoding[" + to_string(textEncoding) + "] not supported!", __FILE__, __func__, __LINE__);
        return;
    }

    // Page Size;
    this->pageSize = ((pageSize == 1)?65536:pageSize);

    // Reserved Space Per Page;
    this->reservedSpacePerPage = structDatabaseHeader.reservedSpacePerPage;

    // Pages;
    this->pages = pages;

    // Text Encoding;
    this->textEncoding = textEncoding;

    this->dbFilename = dbFilename;
}

void Database::close()
{
    if(this->dbStream.is_open())
    {
        this->dbStream.close();
    }
    this->dbFilename = "";
}

bool Database::isOpen() const noexcept
{
    return this->dbStream.is_open();
}

string Database::getDbFilename() const noexcept
{
    return this->dbFilename;
}

uint32_t Database::getPageSize() const noexcept
{
    return this->pageSize;
}

uint16_t Database::getReservedSpacePerPage() const noexcept
{
    return this->reservedSpacePerPage;
}

uint32_t Database::getPages() const noexcept
{
    return this->pages;
}

uint32_t Database::getTextEncoding() const noexcept
{
    return this->textEncoding;
}

void Database::readPage(uint32_t pageNumber, char* buff, Incident& incident) noexcept
{
    // ��飻
    if(pageNumber == 0)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "PageNumber[0] is invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    if(pageNumber > this->pages)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "PageNumber to be read[" + to_string(pageNumber) + "] is beyond pages[" + to_string(this->pages) + "]!", __FILE__, __func__, __LINE__);
        return;
    }

    if(!this->dbStream.is_open())
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "Database unopened!", __FILE__, __func__, __LINE__);
        return;
    }

    // ��ȡ��
    this->dbStream.seekg ((pageNumber - 1) * this->pageSize, ios::beg);
    this->dbStream.read (buff, this->pageSize);
    if(!this->dbStream)
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "Database read fail!", __FILE__, __func__, __LINE__);
        return;
    }
    incident.reset();
}

void Database::readPages(uint32_t pageNumberStart, uint32_t readPageNum, char* buff, Incident& incident) noexcept
{
    // ��飻
    if(pageNumberStart == 0)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "pageNumberStart[0] is invalid!", __FILE__, __func__, __LINE__);
        return;
    }

    if(pageNumberStart + readPageNum - 1 > this->pages)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "pageNumberStart[" + to_string(pageNumberStart) + "], readPageNum[" + to_string(readPageNum) + "] is beyond pages[" + to_string(this->pages) + "]!", __FILE__, __func__, __LINE__);
        return;
    }

    if(!this->dbStream.is_open())
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "Database unopened!", __FILE__, __func__, __LINE__);
        return;
    }

    // ��ȡ��
    this->dbStream.seekg ((pageNumberStart - 1) * this->pageSize, ios::beg);
    this->dbStream.read (buff, this->pageSize * readPageNum);
    if(!this->dbStream)
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "Database read fail!", __FILE__, __func__, __LINE__);
        return;
    }
    incident.reset();
}

shared_ptr<char> Database::getData(Incident& incident) noexcept
{
    incident.reset();
    if(this->data == nullptr)
    {
        this->data = shared_ptr<char>(new char[this->getPages() * this->getPageSize()], default_delete<char[]>());
        this->readPages(1, this->getPages(), this->data.get(), incident);
        if(!incident.succeeded())
        {
            this->data = nullptr;
        }
    }
    return this->data;
}

ostream& hudun::sqlite::operator<<(ostream& out, Database const& database) noexcept
{
    out << "DbFilename: " << database.getDbFilename() << endl;
    out << "Page Size: " << database.getPageSize() << endl;
    out << "Reserved Space: " << database.getReservedSpacePerPage() << endl;
    out << "Pages: " << database.getPages() << endl;
    out << "TextEncoding: " << database.getTextEncoding() << endl;
    return out;
}

