#ifndef HUDUN_SQLITE_SQLITEFACILITY_H
#define HUDUN_SQLITE_SQLITEFACILITY_H

#include <string>
#include <memory>
#include <vector>
using namespace std;

#include "Incident.h"
namespace hudun
{
namespace sqlite
{
class SqliteFacility
{
public:
    static int64_t parseVarint(char const* buff, uint32_t buffLength, uint32_t& varintBytes, hudun::common::Incident& incident) noexcept;
    static vector<int64_t> parseVarints(char const* buff, uint32_t buffLength, uint32_t varintNum, uint32_t& varintsBytes, hudun::common::Incident& incident) noexcept;
    static void serializeVarint(int64_t value, char* buff, uint32_t& varintBytes) noexcept;

    static bool isFieldTypeInteger(int64_t fieldType) noexcept;
    static bool isFieldTypeReal(int64_t fieldType) noexcept;
    static bool isFieldTypeText(int64_t fieldType) noexcept;
    static bool isFieldTypeBlob(int64_t fieldType) noexcept;

    static int64_t parseIntegerValue(char* buff, uint32_t buffLength, int64_t fieldType, uint32_t& valueBytes, hudun::common::Incident& incident) noexcept;

    static double parseRealValue(char* buff, uint32_t buffLength, uint32_t& valueBytes, hudun::common::Incident& incident) noexcept;

//    static string parseTextValue(char* buff, uint32_t buffLength, int64_t fieldType, uint32_t& valueBytes, hudun::common::Incident& incident) noexcept;
    static string parseTextValue(char* buff, uint32_t buffLength, int64_t fieldType, bool acceptPartialValue, bool& partial, uint32_t& valueBytes, hudun::common::Incident& incident) noexcept;

//    static shared_ptr<char> parseBlobValue(char* buff, uint32_t buffLength, int64_t fieldType, uint32_t& valueBytes, hudun::common::Incident& incident) noexcept;
    static shared_ptr<char> parseBlobValue(char* buff, uint32_t buffLength, int64_t fieldType, bool acceptPartialValue, bool& partial, uint32_t& valueBytes, hudun::common::Incident& incident) noexcept;

    static uint64_t parseUnsignedIntegerValue(char* uintBlock, uint32_t uintLength) noexcept;

    static vector<uint32_t> parseCellOffsetArrayInBtreeTablePage(char const* page, uint32_t pageLength, hudun::common::Incident& incident) noexcept;

};
}
}
#endif // HUDUN_SQLITE_SQLITEFACILITY_H

