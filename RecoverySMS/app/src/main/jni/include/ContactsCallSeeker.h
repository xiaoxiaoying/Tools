#ifndef HUDUN_SQLITE_CONTACTS_CONTACTSCALLSEEKER_H
#define HUDUN_SQLITE_CONTACTS_CONTACTSCALLSEEKER_H
#include <memory>
#include <string>
#include <vector>
using namespace std;

#include "Database.h"
#include "SeekProgressIndicator.h"
#include "ContactsCall.h"
#include "Table.h"

namespace hudun
{
namespace sqlite
{
namespace contacts
{
/**
 * Í¨»°¼ÇÂ¼ËÑË÷Æ÷£»
 *
 * @author Wu Deyou
 * @date 2016-01-18
 */
class ContactsCallSeeker
{
private:
    string sourceDbFilename;
    hudun::sqlite::schema::Table callsTable;

    hudun::sqlite::Database database;
    shared_ptr<char> databaseData;

    hudun::sqlite::SeekProgressIndicator* seekProgressIndicator;
    vector<shared_ptr<ContactsCall>> contactsCalls;

protected:
    void parseCallsTable(hudun::common::Incident& incident);

    void openDatabase(hudun::common::Incident& incident) noexcept;
    void readDatabaseData(hudun::common::Incident& incident) noexcept;

public:
    ContactsCallSeeker(string const& sourceDbFilename) noexcept;
    ContactsCallSeeker(ContactsCallSeeker const&) = delete;
    ContactsCallSeeker& operator=(ContactsCallSeeker const&) = delete;
    ~ContactsCallSeeker();

    void registerSeekProgressIndicator(hudun::sqlite::SeekProgressIndicator* seekProgressIndicator) noexcept;
    void seek(uint32_t threadNum, hudun::common::Incident& incident) noexcept;
    vector<shared_ptr<ContactsCall>> getContactsCalls() const noexcept;
    void reset() noexcept;
};
}
}
}
#endif // HUDUN_SQLITE_CONTACTS_CONTACTSCALLSEEKER_H
