#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

#include "../include/Incident.h"
#include "../include/CommonFacility.h"
using namespace hudun::common;

#include "../include/SqliteFacility.h"
#include "../include/SqliteReturnCode.h"
#include "internal/SqliteStructs.h"
#include "../include/Strto.h"

using namespace hudun::sqlite;

int64_t SqliteFacility::parseVarint(char const* buff, uint32_t buffLength, uint32_t& varintBytes, Incident& incident) noexcept
{
    int64_t value = 0;
    for(uint32_t offset = 0; offset < buffLength; offset ++)
    {
        int64_t n = *(buff + offset) & 0xff;
        if(offset < 8)
        {
            value <<= 7;
            value |= (n & 0x7f);
            if((n & 0x80) != 0x80)
            {
                varintBytes = (offset + 1);
                incident.reset();
                return value;
            }
        }
        else
        {
            value <<= 8;
            value |= n;
            varintBytes = 9;
            incident.reset();
            return value;
        }
    }
    incident.set(SqliteReturnCode::FAIL_OVERFLOW, "parse varint overflow!", __FILE__, __func__, __LINE__);
    return value;
}

vector<int64_t> SqliteFacility::parseVarints(char const* buff, uint32_t buffLength, uint32_t varintNum, uint32_t& varintsBytes, Incident& incident) noexcept
{
    vector<int64_t> varints;
    uint32_t byteLengthPerVarint = 0;
    varintsBytes = 0;
    incident.reset();
    while((varintsBytes < buffLength) && (varints.size() < varintNum))
    {
        int64_t varint = SqliteFacility::parseVarint(buff + varintsBytes, buffLength - varintsBytes, byteLengthPerVarint, incident);
        if(incident.succeeded())
        {
            varints.push_back(varint);
            varintsBytes += byteLengthPerVarint;
        }
        else
        {
            break;
        }
    }
    return varints;
}

void SqliteFacility::serializeVarint(int64_t value, char* buff, uint32_t& varintBytes) noexcept
{
    int n = 1;
    while(((value >> (n * 7)) != 0) && (n < 9)) n ++;

    if(n >= 9)
    {
        varintBytes = 9;
        buff[8] = (value & 0xff);
        value >>= 8;
        n = 7;
    }
    else
    {
        varintBytes = n--;
        buff[n] = (value & 0x7f);

        value >>= 7;
        n --;
    }

    while(n >= 0)
    {
        buff[n] = (0x80 | (value & 0x7f));
        value >>= 7;
        n --;
    }
}

bool SqliteFacility::isFieldTypeInteger(int64_t fieldType) noexcept
{
    return ((fieldType > 0) && (fieldType <= 9) && (fieldType != 7));
}

bool SqliteFacility::isFieldTypeReal(int64_t fieldType) noexcept
{
    return (fieldType == 7);
}

bool SqliteFacility::isFieldTypeText(int64_t fieldType) noexcept
{
    return ((fieldType >= 13) && ((fieldType & 1) == 1));
}

bool SqliteFacility::isFieldTypeBlob(int64_t fieldType) noexcept
{
    return ((fieldType >= 12) && ((fieldType & 1) == 0));
}

int64_t SqliteFacility::parseIntegerValue(char* buff, uint32_t buffLength, int64_t fieldType, uint32_t& valueBytes, Incident& incident) noexcept
{
    static int integerBytes[] = {0, 1, 2, 3, 4, 6, 8};
    incident.reset();
    if(fieldType == 0)
    {
        valueBytes = 0;
        return 0;
    }
    else if((fieldType >= 1) && (fieldType <= 6))
    {
        valueBytes = integerBytes[fieldType];
        if(valueBytes > buffLength)
        {
            incident.set(SqliteReturnCode::FAIL_OVERFLOW, "INTEGER required valueBytes[" + to_string(valueBytes)+ "] overflow to buffLength[" + to_string(buffLength) + "]!", __FILE__, __func__, __LINE__);
            return 0;
        }

        int64_t integerValue = (int64_t)buff[0];
        for(uint32_t i = 1; i < valueBytes; i ++)
        {
            integerValue <<= 8;
            integerValue |= (uint8_t)buff[i];
        }
        return integerValue;
    }
    else if(fieldType == 8)
    {
        valueBytes = 0;
        return 0;
    }
    else if(fieldType == 9)
    {
        valueBytes = 0;
        return 1;
    }
    else
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "FieldType[" + to_string(fieldType)+ "] not INTEGER!", __FILE__, __func__, __LINE__);
        return 0;
    }
}

double SqliteFacility::parseRealValue(char* buff, uint32_t buffLength, uint32_t& valueBytes, Incident& incident) noexcept
{
    if(buffLength < 8)
    {
        incident.set(SqliteReturnCode::FAIL_OVERFLOW, "REAL required valueBytes[8] overflow to buffLength[" + to_string(buffLength) + "]!", __FILE__, __func__, __LINE__);
        return 0.0;
    }
    else
    {

        double realValue;
        memcpy((char*)&realValue, buff, 8);
        if(!CommonFacility::isBigEndian())
        {
            CommonFacility::reverse((char*)&realValue, 8);
        }
        valueBytes = 8;
        incident.reset();
        return realValue;
    }
}

/*
string SqliteFacility::parseTextValue(char* buff, uint32_t buffLength, int64_t fieldType, uint32_t& valueBytes, Incident& incident) noexcept
{
    if(SqliteFacility::isFieldTypeText(fieldType))
    {
        valueBytes = ((fieldType - 13) >> 1);
        if(valueBytes > buffLength)
        {
            incident.set(SqliteReturnCode::FAIL_OVERFLOW, "TEXT required valueBytes[" + to_string(valueBytes)+ "] overflow to buffLength[" + to_string(buffLength) + "]!", __FILE__, __func__, __LINE__);
            return "";
        }

        incident.reset();
        return string(buff, valueBytes);
    }
    else
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "FieldType[" + to_string(fieldType) + "] not TEXT!", __FILE__, __func__, __LINE__);
        return "";
    }
}
*/
string SqliteFacility::parseTextValue(char* buff, uint32_t buffLength, int64_t fieldType, bool acceptPartialValue, bool& partial, uint32_t& valueBytes, Incident& incident) noexcept
{
    if(!SqliteFacility::isFieldTypeText(fieldType))
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "FieldType[" + to_string(fieldType) + "] not TEXT!", __FILE__, __func__, __LINE__);
        return "";
    }

    // �����ֶ�ֵ�ĳ��ȣ�
    uint32_t wholeValueBytes = (uint32_t)((fieldType - 13) >> 1);

    // ��������ֶ�ֵ���ڻ����У�
    if(wholeValueBytes <= buffLength)
    {
        valueBytes = wholeValueBytes;
        partial = false;
        incident.reset();
        return string(buff, valueBytes);
    }

    // ���ֻ�в����ֶ�ֵ���ڻ����У�
    else
    {
        if(acceptPartialValue)
        {
            valueBytes = buffLength;
            partial = true;
            incident.reset();
            return string(buff, valueBytes);
        }
        else
        {
            incident.set(SqliteReturnCode::FAIL_OVERFLOW, "TEXT required wholeValueBytes[" + to_string(wholeValueBytes)+ "] overflow to buffLength[" + to_string(buffLength) + "]!", __FILE__, __func__, __LINE__);
            return "";
        }
    }
}
/*
shared_ptr<char> SqliteFacility::parseBlobValue(char* buff, uint32_t buffLength, int64_t fieldType, uint32_t& valueBytes, Incident& incident) noexcept
{
    if(SqliteFacility::isFieldTypeBlob(fieldType))
    {
        valueBytes = ((fieldType - 12) >> 1);
        if(valueBytes > buffLength)
        {
            incident.set(SqliteReturnCode::FAIL_OVERFLOW, "BLOB required valueBytes[" + to_string(valueBytes)+ "] overflow to buffLength[" + to_string(buffLength) + "]!", __FILE__, __func__, __LINE__);
            return nullptr;
        }

        incident.reset();
        shared_ptr<char> blobValue(new char[valueBytes], default_delete<char[]>());
        memcpy(blobValue.get(), buff, valueBytes);
        return blobValue;
    }
    else
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "FieldType[" + to_string(fieldType) + "] not BLOB!", __FILE__, __func__, __LINE__);
        return nullptr;
    }
}
*/

shared_ptr<char> SqliteFacility::parseBlobValue(char* buff, uint32_t buffLength, int64_t fieldType, bool acceptPartialValue, bool& partial, uint32_t& valueBytes, Incident& incident) noexcept
{
    if(!SqliteFacility::isFieldTypeBlob(fieldType))
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "FieldType[" + to_string(fieldType) + "] not BLOB!", __FILE__, __func__, __LINE__);
        return nullptr;
    }

    // �����ֶ�ֵ�ĳ��ȣ�
    uint32_t wholeValueBytes = (uint32_t)((fieldType - 12) >> 1);

    // ��������ֶ�ֵ���ڻ����У�
    if(wholeValueBytes <= buffLength)
    {
        valueBytes = wholeValueBytes;
        partial = false;
        shared_ptr<char> blobValue(new char[valueBytes], default_delete<char[]>());
        memcpy(blobValue.get(), buff, valueBytes);
        incident.reset();
        return blobValue;
    }

    // ���ֻ�в����ֶ�ֵ���ڻ����У�
    else
    {
        if(acceptPartialValue)
        {
            valueBytes = buffLength;
            partial = true;
            shared_ptr<char> blobValue(new char[valueBytes], default_delete<char[]>());
            memcpy(blobValue.get(), buff, valueBytes);
            incident.reset();
            return blobValue;
        }
        else
        {
            incident.set(SqliteReturnCode::FAIL_OVERFLOW, "BLOB required wholeValueBytes[" + to_string(wholeValueBytes)+ "] overflow to buffLength[" + to_string(buffLength) + "]!", __FILE__, __func__, __LINE__);
            return nullptr;
        }
    }
}

uint64_t SqliteFacility::parseUnsignedIntegerValue(char* uintBlock, uint32_t uintLength) noexcept
{
    uint64_t uintValue = (uint8_t)uintBlock[0];
    for(uint32_t i = 1; i < uintLength; i ++)
    {
        uintValue <<= 8;
        uintValue |= (uint8_t)uintBlock[i];
    }
    return uintValue;
}

vector<uint32_t> SqliteFacility::parseCellOffsetArrayInBtreeTablePage(char const* page, uint32_t pageLength, Incident& incident) noexcept
{
    vector<uint32_t> cellOffsetArray;
    StructPageBtreeHeader* pbh = (StructPageBtreeHeader*)page;

    // ���ҳ�����ͱ�־��
    if(pbh->pageFlag != '\x0d')
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "PageFlag[" + CommonFacility::toHexString(pbh->pageFlag & 0xff) + "] not BtreeTablePage flag '0x0d'!", __FILE__, __func__, __LINE__);
        return cellOffsetArray;
    }

    // ����¼������
    uint32_t cells = CommonFacility::be16toh(pbh->cells) & 0xffff;
    if((8 + (cells << 1)) >= pageLength)
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "cells[" + to_string(cells) + "] too big!", __FILE__, __func__, __LINE__);
        return cellOffsetArray;
    }

    // ���������ƫ�ƣ�
    uint32_t contentOffset = CommonFacility::be16toh(pbh->contentOffset) & 0xffff;
    if((contentOffset <= 10) || (contentOffset >= pageLength))
    {
        incident.set(SqliteReturnCode::FAIL_GENERAL, "contentOffset[" + to_string(contentOffset) + "] invalid!", __FILE__, __func__, __LINE__);
        return cellOffsetArray;
    }

    // ��ȡ��¼ƫ�����飻
    for(uint32_t i = 0; i < cells; i ++)
    {
        uint16_t* p = (uint16_t*)(page + 8 + (i << 1));
        uint32_t offset = CommonFacility::be16toh(*p) & 0xffff;
        if((offset < contentOffset) || (offset >= pageLength))
        {
            incident.set(SqliteReturnCode::FAIL_GENERAL, "offset[" + to_string(offset) + "] not in[" + to_string(contentOffset) + ", " + to_string(pageLength) + ")!", __FILE__, __func__, __LINE__);
            cellOffsetArray.clear();
            return cellOffsetArray;
        }
        cellOffsetArray.push_back(offset);
    }
    return cellOffsetArray;
}
