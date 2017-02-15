#ifndef HUDUN_SQLITE_CONTACTS_CONTACTSCALL_H
#define HUDUN_SQLITE_CONTACTS_CONTACTSCALL_H
#include <iostream>
#include <memory>
#include <string>
using namespace std;

#include "Entity.h"
#include "Record.h"

namespace hudun
{
namespace sqlite
{
namespace contacts
{
class ContactsCall: public hudun::sqlite::Entity
{
public:
    static const uint64_t ATTRFLAG_NONE             = 0;
    static const uint64_t ATTRFLAG_ID               = 0x00000001;
    static const uint64_t ATTRFLAG_NUMBER           = 0x00000002;
    static const uint64_t ATTRFLAG_DATE             = 0x00000004;
    static const uint64_t ATTRFLAG_DURATION         = 0x00000008;
    static const uint64_t ATTRFLAG_TYPE             = 0x00000010;
    static const uint64_t ATTRFLAG_ISNEW            = 0x00000020;
    static const uint64_t ATTRFLAG_NAME             = 0x00000040;
    static const uint64_t ATTRFLAG_NUMBERTYPE       = 0x00000080;
    static const uint64_t ATTRFLAG_NUMBERLABEL      = 0x00000100;
    static const uint64_t ATTRFLAG_COUNTRYISO       = 0x00000200;
    static const uint64_t ATTRFLAG_VOICEMAILURI     = 0x00000400;
    static const uint64_t ATTRFLAG_ISREAD           = 0x00000800;
    static const uint64_t ATTRFLAG_GEOCODEDLOCATION = 0x00001000;
    static const uint64_t ATTRFLAG_LOOKUPURI        = 0x00002000;
    static const uint64_t ATTRFLAG_MATCHEDNUMBER    = 0x00004000;
    static const uint64_t ATTRFLAG_NORMALIZEDNUMBER = 0x00008000;
    static const uint64_t ATTRFLAG_PHOTOID          = 0x00010000;
    static const uint64_t ATTRFLAG_FORMATTEDNUMBER  = 0x00020000;
    static const uint64_t ATTRFLAG_DATA             = 0x00040000;
    static const uint64_t ATTRFLAG_HASCONTENT       = 0x00080000;
    static const uint64_t ATTRFLAG_MIMETYPE         = 0x00100000;
    static const uint64_t ATTRFLAG_SOURCEDATA       = 0x00200000;
    static const uint64_t ATTRFLAG_SOURCEPACKAGE    = 0x00400000;
    static const uint64_t ATTRFLAG_STATE            = 0x00800000;
    static const uint64_t ATTRFLAG_SUBSCRIPTION     = 0x01000000;
    static const uint64_t ATTRFLAG_ISPRIVATE        = 0x02000000;
    static const uint64_t ATTRFLAG_RINGTIMES        = 0x04000000;
    static const uint64_t ATTRFLAG_PAGENUMBER       = 0x08000000;

protected:
    int64_t id;
    string number;
    uint64_t date;
    int32_t duration;
    int16_t type;
    int16_t isNew;
    string name;
    int16_t numberType;
    string numberLabel;
    string countryIso;
    string voicemailUri;
    int16_t isRead;
    string geocodedLocation;
    string lookupUri;
    string matchedNumber;
    string normalizedNumber;
    int16_t photoId;
    string formattedNumber;
    string data;
    int16_t hasContent;
    string mimeType;
    string sourceData;
    string sourcePackage;
    int16_t state;
    int16_t subscription;
    int16_t isPrivate;
    int16_t ringTimes;
    uint32_t pageNumber;

public:
    ContactsCall(uint16_t obtainWay) noexcept;
    void setId(int64_t id) noexcept;
    void setNumber(string const& number) noexcept;
    void setDate(uint64_t date) noexcept;
    void setDuration(int32_t duration) noexcept;
    void setType(int16_t type) noexcept;
    void setIsNew(int16_t isNew) noexcept;
    void setName(string const& name) noexcept;
    void setNumberType(int16_t numberType) noexcept;
    void setNumberLabel(string const& numberLabel) noexcept;
    void setCountryIso(string const& countryIso) noexcept;
    void setVoicemailUri(string const& voicemailUri) noexcept;
    void setIsRead(int16_t isRead) noexcept;
    void setGeocodedLocation(string const& geocodedLocation) noexcept;
    void setLookupUri(string const& lookupUri) noexcept;
    void setMatchedNumber(string const& matchedNumber) noexcept;
    void setNormalizedNumber(string const& normalizedNumber) noexcept;
    void setPhotoId(int16_t photoId) noexcept;
    void setFormattedNumber(string const& formattedNumber) noexcept;
    void setData(string const& data) noexcept;
    void setHasContent(int16_t hasContent) noexcept;
    void setMimeType(string const& mimeType) noexcept;
    void setSourceData(string const& sourceData) noexcept;
    void setSourcePackage(string const& sourcePackage) noexcept;
    void setState(int16_t state) noexcept;
    void setSubscription(int16_t subscription) noexcept;
    void setIsPrivate(int16_t isPrivate) noexcept;
    void setRingTimes(int16_t ringTimes) noexcept;
    void setPageNumber(uint32_t pageNumber) noexcept;

    int64_t getId() const noexcept;
    string getNumber() const noexcept;
    uint64_t getDate() const noexcept;
    int32_t getDuration() const noexcept;
    int16_t getType() const noexcept;
    int16_t getIsNew() const noexcept;
    string getName() const noexcept;
    int16_t getNumberType() const noexcept;
    string getNumberLabel() const noexcept;
    string getCountryIso() const noexcept;
    string getVoicemailUri() const noexcept;
    int16_t getIsRead() const noexcept;
    string getGeocodedLocation() const noexcept;
    string getLookupUri() const noexcept;
    string getMatchedNumber() const noexcept;
    string getNormalizedNumber() const noexcept;
    int16_t getPhotoId() const noexcept;
    string getFormattedNumber() const noexcept;
    string getData() const noexcept;
    int16_t getHasContent() const noexcept;
    string getMimeType() const noexcept;
    string getSourceData() const noexcept;
    string getSourcePackage() const noexcept;
    int16_t getState() const noexcept;
    int16_t getSubscription() const noexcept;
    int16_t getIsPrivate() const noexcept;
    int16_t getRingTimes() const noexcept;
    uint32_t getPageNumber() const noexcept;

    static shared_ptr<ContactsCall> translate(hudun::sqlite::Record const& record) noexcept;
};
}
}
}
ostream& operator<<(ostream& out, hudun::sqlite::contacts::ContactsCall const& contactsCall) noexcept;
#endif // HUDUN_SQLITE_CONTACTS_CONTACTSCALL_H
