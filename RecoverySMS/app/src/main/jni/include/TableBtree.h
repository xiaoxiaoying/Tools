#ifndef HUDUN_SQLITE_TABLEBTREE_H
#define HUDUN_SQLITE_TABLEBTREE_H
#include <string>
#include <set>
using namespace std;

#include "Incident.h"
#include "Database.h"
#include "Entity.h"

namespace hudun
{
namespace sqlite
{
class TableBtree: public Entity
{
public:
    static const uint64_t FIELD_FLAG_NONE           = 0;
    static const uint64_t FIELD_FLAG_TYPE           = 0x0001;
    static const uint64_t FIELD_FLAG_NAME           = 0x0002;
    static const uint64_t FIELD_FLAG_TBL_NAME       = 0x0004;
    static const uint64_t FIELD_FLAG_ROOTPAGE       = 0x0008;
    static const uint64_t FIELD_FLAG_SQL            = 0x0010;

private:
    string tblName;
    uint32_t rootPage;

    set<uint32_t> interiorPageNumbers;
    set<uint32_t> leafPageNumbers;

public:
    TableBtree(uint16_t obtainWay) noexcept;

    void setTblName(string const& tblName) noexcept;
    void setRootPage(uint32_t rootPage) noexcept;

    string getTblName() const noexcept;
    uint32_t getRootPage() const noexcept;

    void collect(Database& database, hudun::common::Incident& incident) noexcept;
    const set<uint32_t>& getInteriorPageNumbers() const noexcept;
    const set<uint32_t>& getLeafPageNumbers() const noexcept;
};
}
}
#endif // HUDUN_SQLITE_TABLEBTREE_H



