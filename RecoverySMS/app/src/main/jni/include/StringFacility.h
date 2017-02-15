#ifndef HUDUN_COMMON_STRINGFACILITY_H
#define HUDUN_COMMON_STRINGFACILITY_H

#include <string>
#include <vector>
using namespace std;

namespace hudun
{
namespace common
{
class StringFacility
{
public:
    static const char* WHITESPACE;

public:
    static void split(string const s, vector<string>& tokens, string const& delimiters) noexcept;
    static void ltrim(string& s, string const& wsChars) noexcept;
    static void rtrim(string& s, string const& wsChars) noexcept;
    static void trim(string& s, string const& wsChars) noexcept;

    static void toupper(string& s) noexcept;
    static void tolower(string& s) noexcept;

    static bool match(string const& s, string const& pattern, bool ignoreCase);
    static bool startsWith(string const& s, string const& start, bool ignoreCase);
    static bool endsWith(string const& s, string const& end, bool ignoreCase);
    static int findAndReplace(string& s, string const& f, string const& r);

//    static string trimNonUtf8(string const& s) noexcept;

    static vector<uint32_t> transformFromUtf8ToUnicode(const char* s, size_t length, uint32_t& invalidBytes) noexcept;
    static string transformFromUnicodeToUtf8(vector<uint32_t> const& unicodes) noexcept;


    static bool isUcs2Printable(uint16_t unicode) noexcept;
    static string washUtf8(string const s) noexcept;

};
}
}

#endif // HUDUN_COMMON_STRINGFACILITY_H
