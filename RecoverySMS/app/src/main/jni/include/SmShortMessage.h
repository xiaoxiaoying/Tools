#ifndef HUDUN_SQLITE_SM_SMSHORTMESSAGE_H
#define HUDUN_SQLITE_SM_SMSHORTMESSAGE_H
#include <memory>
#include <string>
using namespace std;

#include "Entity.h"
#include "Record.h"

namespace hudun
{
namespace sqlite
{
namespace sm
{
class SmShortMessage: public hudun::sqlite::Entity
{
public:
    static const uint64_t ATTRFLAG_NONE           = 0;
    static const uint64_t ATTRFLAG_SMID             = 0x000001;
    static const uint64_t ATTRFLAG_THREADID         = 0x000002;
    static const uint64_t ATTRFLAG_ADDRESS          = 0x000004;
    static const uint64_t ATTRFLAG_MSIZE            = 0x000008;
    static const uint64_t ATTRFLAG_PERSON           = 0x000010;
    static const uint64_t ATTRFLAG_DATE             = 0x000020;
    static const uint64_t ATTRFLAG_DATESENT         = 0x000040;
    static const uint64_t ATTRFLAG_PROTOCOL         = 0x000080;
    static const uint64_t ATTRFLAG_READ             = 0x000100;
    static const uint64_t ATTRFLAG_STATUS           = 0x000200;
    static const uint64_t ATTRFLAG_TYPE             = 0x000400;
    static const uint64_t ATTRFLAG_REPLYPATHPRESENT = 0x000800;
    static const uint64_t ATTRFLAG_SUBJECT          = 0x001000;
    static const uint64_t ATTRFLAG_BODY             = 0x002000;
    static const uint64_t ATTRFLAG_SERVICECENTER    = 0x004000;
    static const uint64_t ATTRFLAG_LOCKED           = 0x008000;
    static const uint64_t ATTRFLAG_SUBID            = 0x010000;
    static const uint64_t ATTRFLAG_REFERENCENUMBER  = 0x020000;
    static const uint64_t ATTRFLAG_ERRORCODE        = 0x040000;
    static const uint64_t ATTRFLAG_SEEN             = 0x080000;
    static const uint64_t ATTRFLAG_PRI              = 0x100000;
    static const uint64_t ATTRFLAG_IMSI             = 0x200000;
    static const uint64_t ATTRFLAG_PAGENUMBER       = 0x400000;

protected:
    int64_t smId;
    int threadId;
    string address;
    int mSize;
    int person;
    int64_t date;
    int64_t dateSent;
    int protocol;
    int read;
    int status;
    int type;
    int replyPathPresent;
    string subject;
    string body;
    string serviceCenter;
    int locked;
    int64_t subId;
    int referenceNumber;
    int errorCode;
    int seen;
    int pri;
    string imsi;

    uint32_t pageNumber;

public:
    SmShortMessage(uint16_t obtainWay) noexcept;

    void setSmId(int64_t smId) noexcept;
    void setThreadId(int threadId) noexcept;
    void setAddress(string const& address) noexcept;
    void setMSize(int mSize) noexcept;
    void setPerson(int person) noexcept;
    void setDate(int64_t date) noexcept;
    void setDateSent(int64_t dateSent) noexcept;
    void setProtocol(int protocol) noexcept;
    void setRead(int read) noexcept;
    void setStatus(int status) noexcept;
    void setType(int type) noexcept;
    void setReplyPathPresent(int replyPathPresent) noexcept;
    void setSubject(string const& subject) noexcept;
    void setBody(string const& body) noexcept;
    void setServiceCenter(string const& serviceCenter) noexcept;
    void setLocked(int locked) noexcept;
    void setSubId(int64_t subId) noexcept;
    void setReferenceNumber(int referenceNumber) noexcept;
    void setErrorCode(int errorCode) noexcept;
    void setSeen(int seen) noexcept;
    void setPri(int pri) noexcept;
    void setImsi(string const& imsi) noexcept;
    void setPageNumber(uint32_t pageNumber) noexcept;

    int64_t getSmId() const noexcept;
    int getThreadId() const noexcept;
    string getAddress() const noexcept;
    int getMSize() const noexcept;
    int getPerson() const noexcept;
    int64_t getDate() const noexcept;
    int64_t getDateSent() const noexcept;
    int getProtocol() const noexcept;
    int getRead() const noexcept;
    int getStatus() const noexcept;
    int getType() const noexcept;
    int getReplyPathPresent() const noexcept;
    string getSubject() const noexcept;
    string getBody() const noexcept;
    string getServiceCenter() const noexcept;
    int getLocked() const noexcept;
    int64_t getSubId() const noexcept;
    int getReferenceNumber() const noexcept;
    int getErrorCode() const noexcept;
    int getSeen() const noexcept;
    int getPri() const noexcept;
    string getImsi() const noexcept;
    uint32_t getPageNumber() const noexcept;


    static shared_ptr<SmShortMessage> translate(hudun::sqlite::Record const& record) noexcept;
};
}
}
}
ostream& operator<<(ostream& out, hudun::sqlite::sm::SmShortMessage const& smShortMessage) noexcept;
#endif // HUDUN_SQLITE_SM_SMSHORTMESSAGE_H
