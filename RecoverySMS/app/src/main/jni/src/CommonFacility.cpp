#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

#include <unistd.h>


#include "../include/CommonException.h"
#include "../include/CommonFacility.h"
#include "../include/Incident.h"
using namespace hudun::common;


static const uint16_t __endian__ = 1;
const bool CommonFacility::IS_BIG_ENDIAN =  (*((char*)&__endian__) == '\0');


bool CommonFacility::isBigEndian() noexcept
{
    return CommonFacility::IS_BIG_ENDIAN;
}

void CommonFacility::reverse(char* buff, uint16_t length) noexcept
{
    for(int i = 0; i < (length >> 1); i ++)
    {
        char c = buff[i];
        buff[i] = buff[length - i - 1];
        buff[length - i - 1] = c;
    }
}

uint16_t CommonFacility::htobe16(uint16_t h) noexcept
{
    if(!CommonFacility::isBigEndian()) CommonFacility::reverse((char*)&h, 2);
    return h;
}

uint16_t CommonFacility::htole16(uint16_t h) noexcept
{
    if(CommonFacility::isBigEndian()) CommonFacility::reverse((char*)&h, 2);
    return h;
}

uint16_t CommonFacility::be16toh(uint16_t b) noexcept
{
    return htobe16(b);
}

uint16_t CommonFacility::le16toh(uint16_t l) noexcept
{
    return htole16(l);
}

uint32_t CommonFacility::htobe32(uint32_t h) noexcept
{
    if(!CommonFacility::isBigEndian()) CommonFacility::reverse((char*)&h, 4);
    return h;
}

uint32_t CommonFacility::htole32(uint32_t h) noexcept
{
    if(CommonFacility::isBigEndian()) CommonFacility::reverse((char*)&h, 4);
    return h;
}

uint32_t CommonFacility::be32toh(uint32_t b) noexcept
{
    return htobe32(b);
}

uint32_t CommonFacility::le32toh(uint32_t l) noexcept
{
    return htole32(l);
}

uint64_t CommonFacility::htobe64(uint64_t h) noexcept
{
    if(!CommonFacility::isBigEndian()) CommonFacility::reverse((char*)&h, 8);
    return h;
}

uint64_t CommonFacility::htole64(uint64_t h) noexcept
{
    if(CommonFacility::isBigEndian()) CommonFacility::reverse((char*)&h, 8);
    return h;
}

uint64_t CommonFacility::be64toh(uint64_t b) noexcept
{
    return htobe64(b);
}

uint64_t CommonFacility::le64toh(uint64_t l) noexcept
{
    return htole64(l);
}

/*
void CommonFacility::convertFromBigEndian(int8_t&) noexcept
{
}

void CommonFacility::convertFromBigEndian(int16_t& n) noexcept
{
    if(!CommonFacility::IS_BIG_ENDIAN) reverse((char*)&n, 2);
}

void CommonFacility::convertFromBigEndian(int32_t& n) noexcept
{
    if(!CommonFacility::IS_BIG_ENDIAN) reverse((char*)&n, 4);
}

void CommonFacility::convertFromBigEndian(int64_t& n) noexcept
{
    if(!CommonFacility::IS_BIG_ENDIAN) reverse((char*)&n, 8);
}

void CommonFacility::convertFromBigEndian(uint8_t&) noexcept
{
}

void CommonFacility::convertFromBigEndian(uint16_t& n) noexcept
{
    if(!CommonFacility::IS_BIG_ENDIAN) reverse((char*)&n, 2);
}

void CommonFacility::convertFromBigEndian(uint32_t& n) noexcept
{
    if(!CommonFacility::IS_BIG_ENDIAN) reverse((char*)&n, 4);
}

void CommonFacility::convertFromBigEndian(uint64_t& n) noexcept
{
    if(!CommonFacility::IS_BIG_ENDIAN) reverse((char*)&n, 8);
}
*/
string CommonFacility::toHexString(int64_t n) noexcept
{
    char buff[32];
    bool negative = false;
    if(n < 0)
    {
        negative = true;
        n = -n;
    }

    uint16_t offset = 0;
    if(negative)
    {
        buff[0] = '-';
        offset = 1;
    }


    uint32_t uh = (n >> 32);
    uint32_t ul = (uint32_t)(n & 0xffffffff);
    if(uh != 0)
    {
        sprintf(buff + offset, "%08X", uh);
        offset += 8;
    }
    sprintf(buff + offset, "%08X", ul);

    return string(buff);
}

string CommonFacility::toHexString(const char* buff, uint32_t buffLength) noexcept
{
    char* temp = new char[(buffLength << 1) + 1];
    for(uint32_t i = 0; i < buffLength; i ++)
    {

        sprintf(temp + (i << 1), "%02x", buff[i] & 0xff);
    }
    temp[(buffLength << 1)] = '\0';
    string hex = string(temp);
    delete[] temp;
    return hex;
}

void CommonFacility::parseHexString(const string& hexString, char* buff) noexcept
{
    const char* hs = hexString.c_str();
    unsigned int n;
    for(uint32_t i = 0; i < (hexString.length() >> 1); i ++)
    {

        sscanf(hs + i + i,"%02x",  &n);
        buff[i] = (char)n;
    }
}

CommonException CommonFacility::translate(Incident const& incident) noexcept
{
    return CommonException(incident.getCode(), incident.getMessage(), incident.getFilename(), incident.getFunctionName(), incident.getLineNo());
}

bool CommonFacility::existFile(string const& filename) noexcept
{
#ifdef _MSC_VER
    int16_t outFsFlags;
    return Win32Facility::exists(filename, Win32Facility::FsFlag::ANY, outFsFlags);
#else
    return (access(filename.c_str(), F_OK ) != -1);
#endif // _MSC_VER

}

void CommonFacility::removeFile(string const& filename, Incident& incident) noexcept
{
#ifdef _MSC_VER
    Win32Facility::deleteFile(filename, incident);
#else
    int returnCode = remove(filename.c_str());
    if(returnCode == 0)
    {
        incident.reset();
    }
    else
    {
        incident.set(Incident::FAIL_GENERAL, strerror(errno), __FILE__, __func__, __LINE__);
    }
#endif // _MSC_VER
}

bool CommonFacility::parseCmdLineParameters(int argc, char* argv[],  map<string, int> const& optionFlags, vector<string>& parameters, map<string, string>& options, string& errorMessage) noexcept
{
    int i = 0;
    while(i < argc)
    {
        if(argv[i][0] == '-')
        {
            // ���'-'��ʼ�Ĳ�������ѡ� ����Ϊ������
            auto optionFlagsIt = optionFlags.find(argv[i]);
            if(optionFlagsIt == optionFlags.end())
            {
                parameters.push_back(argv[i]);
                i ++;
                continue;
            }

            // ���ѡ���Ƿ��ظ���
            auto optionsIt = options.find(argv[i]);
            if(optionsIt != options.end())
            {
                errorMessage = string("option '") + argv[i] + "' repeated!";
                return false;
            }

            // ��ֵѡ�
            if(optionFlagsIt->second == 0)
            {
                options.emplace(argv[i], "");
                i ++;
                continue;
            }
            // ��ֵѡ�
            else if(optionFlagsIt->second == 1)
            {
                if(i + 1 < argc)
                {
                    options.emplace(argv[i], argv[i+1]);
                    i += 2;
                    continue;
                }
                else
                {
                    errorMessage = string("option '") + argv[i] + "' invalid!";
                    return false;
                }
            }
            else
            {
                errorMessage = string("option '") + argv[i] + "' flags error!";
                return false;
            }
        }
        else
        {
            parameters.push_back(argv[i]);
            i ++;
        }
    }
    return true;
}

string CommonFacility::formatCapacity(uint64_t capacity) noexcept
{
    uint64_t kib = capacity / 1024;
    uint64_t mib = kib / 1024;
    kib = capacity % 1024;
    uint64_t gib = mib / 1024;
    mib %= 1024;

    char buff[128];
    if(gib > 0)
    {
        sprintf(buff, "%lluGib%lluMib", gib, mib);
    }
    else if(mib > 0)
    {
        sprintf(buff, "%lluMib%lluKib", mib, kib);
    }
    else
    {
        sprintf(buff, "%lluKib", kib);
    }

    return string(buff);
}

string CommonFacility::formatCapacityToFloat(uint64_t capacity) noexcept
{
    static const uint64_t K = 1024;
    static const uint64_t M = 1024 * K;
    static const uint64_t G = 1024 * M;
    static const uint64_t T = 1024 * G;

    string unit;
    float f;
    if(capacity <= K)
    {
        f = (float)capacity;
    }
    else if(capacity <= M)
    {
        f = (float)capacity / K;
        unit = "K";
    }
    else if(capacity <= G)
    {
        f = (float)capacity / M;
        unit = "M";
    }
    else if(capacity <= T)
    {
        f = (float)capacity / G;
        unit = "G";
    }
    else
    {
        f = (float)capacity / T;
        unit = "T";
    }


    char buff[128];
    sprintf(buff, "%.2f%s", f, unit.c_str());

    return string(buff);
}
