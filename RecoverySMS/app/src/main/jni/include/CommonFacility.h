#ifndef HUDUN_COMMON_COMMONFACILITY_H
#define HUDUN_COMMON_COMMONFACILITY_H

#include <string>
#include <vector>
#include <map>
using namespace std;

#include "CommonException.h"
#include "Incident.h"

/*
#ifdef htobe16
#pragma push_macro("htobe16")
#undef htobe16
#define __HUDUNFLAG_DEFINED_htobe16__
#endif // htobe16

#ifdef htole16
#pragma push_macro("htole16")
#undef htole16
#define __HUDUNFLAG_DEFINED_htole16__
#endif // htole16

#ifdef be16toh
#pragma push_macro("be16toh")
#undef be16toh
#define __HUDUNFLAG_DEFINED_be16toh__
#endif // be16toh

#ifdef le16toh
#pragma push_macro("le16toh")
#undef le16toh
#define __HUDUNFLAG_DEFINED_le16toh__
#endif // le16toh

#ifdef htobe32
#pragma push_macro("htobe32")
#undef htobe32
#define __HUDUNFLAG_DEFINED_htobe32__
#endif // htobe32

#ifdef htole32
#pragma push_macro("htole32")
#undef htole32
#define __HUDUNFLAG_DEFINED_htole32__
#endif // htole32

#ifdef be32toh
#pragma push_macro("be32toh")
#undef be32toh
#define __HUDUNFLAG_DEFINED_be32toh__
#endif // be32toh

#ifdef le32toh
#pragma push_macro("le32toh")
#undef le32toh
#define __HUDUNFLAG_DEFINED_le32toh__
#endif // le32toh

#ifdef htobe64
#pragma push_macro("htobe64")
#undef htobe64
#define __HUDUNFLAG_DEFINED_htobe64__
#endif // htobe64

#ifdef htole64
#pragma push_macro("htole64")
#undef htole64
#define __HUDUNFLAG_DEFINED_htole64__
#endif // htole64

#ifdef be64toh
#pragma push_macro("be64toh")
#undef be64toh
#define __HUDUNFLAG_DEFINED_be64toh__
#endif // be64toh

#ifdef le64toh
#pragma push_macro("le64toh")
#undef le64toh
#define __HUDUNFLAG_DEFINED_le64toh__
#endif // le64toh
*/
namespace hudun
{
namespace common
{
class CommonFacility
{
public:
    static const bool IS_BIG_ENDIAN;

public:
    static bool isBigEndian() noexcept;
    static void reverse(char* buff, uint16_t length) noexcept;

    static uint16_t htobe16(uint16_t h) noexcept;
    static uint16_t htole16(uint16_t h) noexcept;
    static uint16_t be16toh(uint16_t b) noexcept;
    static uint16_t le16toh(uint16_t l) noexcept;

    static uint32_t htobe32(uint32_t h) noexcept;
    static uint32_t htole32(uint32_t h) noexcept;
    static uint32_t be32toh(uint32_t b) noexcept;
    static uint32_t le32toh(uint32_t l) noexcept;

    static uint64_t htobe64(uint64_t h) noexcept;
    static uint64_t htole64(uint64_t h) noexcept;
    static uint64_t be64toh(uint64_t b) noexcept;
    static uint64_t le64toh(uint64_t l) noexcept;

    static string toHexString(int64_t n) noexcept;
    static string toHexString(const char* buff, uint32_t buffLength) noexcept;
    static void parseHexString(const string& hexString, char* buff) noexcept;

    static CommonException translate(Incident const& incident) noexcept;

    static bool existFile(string const& filename) noexcept;
    static void removeFile(string const& filename, Incident& incident) noexcept;

    static bool parseCmdLineParameters(int argc, char* argv[], map<string, int> const& optionFlags, vector<string>& parameters, map<string, string>& options, string& errorMessage) noexcept;

    static string formatCapacity(uint64_t capacity) noexcept;
    static string formatCapacityToFloat(uint64_t capacity) noexcept;


};
}
}
/*
#ifdef __HUDUNFLAG_DEFINED_le64toh__
#pragma pop_macro("le64toh")
#undef __HUDUNFLAG_DEFINED_le64toh__
#endif // __HUDUNFLAG_DEFINED_le64toh__

#ifdef __HUDUNFLAG_DEFINED_be64toh__
#pragma pop_macro("be64toh")
#undef __HUDUNFLAG_DEFINED_be64toh__
#endif // __HUDUNFLAG_DEFINED_be64toh__

#ifdef __HUDUNFLAG_DEFINED_htole64__
#pragma pop_macro("htole64")
#undef __HUDUNFLAG_DEFINED_htole64__
#endif // __HUDUNFLAG_DEFINED_htole64__

#ifdef __HUDUNFLAG_DEFINED_htobe64__
#pragma pop_macro("htobe64")
#undef __HUDUNFLAG_DEFINED_htobe64__
#endif // __HUDUNFLAG_DEFINED_htobe64__

#ifdef __HUDUNFLAG_DEFINED_le32toh__
#pragma pop_macro("le32toh")
#undef __HUDUNFLAG_DEFINED_le32toh__
#endif // __HUDUNFLAG_DEFINED_le32toh__

#ifdef __HUDUNFLAG_DEFINED_be32toh__
#pragma pop_macro("be32toh")
#undef __HUDUNFLAG_DEFINED_be32toh__
#endif // __HUDUNFLAG_DEFINED_be32toh__

#ifdef __HUDUNFLAG_DEFINED_htole32__
#pragma pop_macro("htole32")
#undef __HUDUNFLAG_DEFINED_htole32__
#endif // __HUDUNFLAG_DEFINED_htole32__

#ifdef __HUDUNFLAG_DEFINED_htobe32__
#pragma pop_macro("htobe32")
#undef __HUDUNFLAG_DEFINED_htobe32__
#endif // __HUDUNFLAG_DEFINED_htobe32__

#ifdef __HUDUNFLAG_DEFINED_le16toh__
#pragma pop_macro("le16toh")
#undef __HUDUNFLAG_DEFINED_le16toh__
#endif // __HUDUNFLAG_DEFINED_le16toh__

#ifdef __HUDUNFLAG_DEFINED_be16toh__
#pragma pop_macro("be16toh")
#undef __HUDUNFLAG_DEFINED_be16toh__
#endif // __HUDUNFLAG_DEFINED_be16toh__

#ifdef __HUDUNFLAG_DEFINED_htole16__
#pragma pop_macro("htole16")
#undef __HUDUNFLAG_DEFINED_htole16__
#endif // __HUDUNFLAG_DEFINED_htole16__

#ifdef __HUDUNFLAG_DEFINED_htobe16__
#pragma pop_macro("htobe16")
#undef __HUDUNFLAG_DEFINED_htobe16__
#endif // __HUDUNFLAG_DEFINED_htobe16__
*/

#endif // HUDUN_COMMON_COMMONFACILITY_H
