#ifndef HUDUN_SQLITE_SM_SMSHORTMESSAGESEEKER_H
#define HUDUN_SQLITE_SM_SMSHORTMESSAGESEEKER_H
#include <memory>
#include <string>
#include <vector>
using namespace std;

#include "Database.h"
#include "SeekProgressIndicator.h"
#include "SmShortMessage.h"
#include "Table.h"

namespace hudun
{
namespace sqlite
{
namespace sm
{
/**
 * ¶ÌÐÅËÑË÷Æ÷£»
 *
 * @author Wu Deyou
 * @date 2015-12-28
 */
class SmShortMessageSeeker
{
private:
    string sourceDbFilename;
    hudun::sqlite::schema::Table smsTable;

    hudun::sqlite::Database database;
    shared_ptr<char> databaseData;

    hudun::sqlite::SeekProgressIndicator* seekProgressIndicator;
    vector<shared_ptr<SmShortMessage>> smShortMessages;

protected:
    void parseSmsTable(hudun::common::Incident& incident);

    void openDatabase(hudun::common::Incident& incident) noexcept;
    void readDatabaseData(hudun::common::Incident& incident) noexcept;

public:
    SmShortMessageSeeker(string const& sourceDbFilename) noexcept;
    SmShortMessageSeeker(SmShortMessageSeeker const&) = delete;
    SmShortMessageSeeker& operator=(SmShortMessageSeeker const&) = delete;
    ~SmShortMessageSeeker();

    void registerSeekProgressIndicator(hudun::sqlite::SeekProgressIndicator* seekProgressIndicator) noexcept;
    void seek(uint32_t threadNum, hudun::common::Incident& incident) noexcept;
    vector<shared_ptr<SmShortMessage>> getShortMessages() const noexcept;
    void reset() noexcept;
};
}
}
}
#endif // HUDUN_SQLITE_SM_SMSHORTMESSAGESEEKER_H
