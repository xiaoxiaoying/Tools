#include <iostream>
#include <string>
using namespace std;

#include "../include/StringFacility.h"
using namespace hudun::common;

#include "../include/Entity.h"
#include "../include/FieldValue.h"
#include "../include/Record.h"
using namespace hudun::sqlite;

#include "../include/ContactsCall.h"
using namespace hudun::sqlite::contacts;

ContactsCall::ContactsCall(uint16_t obtainWay) noexcept: Entity(obtainWay)
{
}

void ContactsCall::setId(int64_t id) noexcept
{
    this->id = id;
    this->validFlags |= ContactsCall::ATTRFLAG_ID;
}

void ContactsCall::setNumber(string const& number) noexcept
{
    this->number = number;
    this->validFlags |= ContactsCall::ATTRFLAG_NUMBER;
}

void ContactsCall::setDate(uint64_t date) noexcept
{
    this->date = date;
    this->validFlags |= ContactsCall::ATTRFLAG_DATE;
}

void ContactsCall::setDuration(int32_t duration) noexcept
{
    this->duration = duration;
    this->validFlags |= ContactsCall::ATTRFLAG_DURATION;
}

void ContactsCall::setType(int16_t type) noexcept
{
    this->type = type;
    this->validFlags |= ContactsCall::ATTRFLAG_TYPE;
}

void ContactsCall::setIsNew(int16_t isNew) noexcept
{
    this->isNew = isNew;
    this->validFlags |= ContactsCall::ATTRFLAG_ISNEW;
}

void ContactsCall::setName(string const& name) noexcept
{
    this->name = name;
    this->validFlags |= ContactsCall::ATTRFLAG_NAME;
}

void ContactsCall::setNumberType(int16_t numberType) noexcept
{
    this->numberType = numberType;
    this->validFlags |= ContactsCall::ATTRFLAG_NUMBERTYPE;
}

void ContactsCall::setNumberLabel(string const& numberLabel) noexcept
{
    this->numberLabel = numberLabel;
    this->validFlags |= ContactsCall::ATTRFLAG_NUMBERLABEL;
}

void ContactsCall::setCountryIso(string const& countryIso) noexcept
{
    this->countryIso = countryIso;
    this->validFlags |= ContactsCall::ATTRFLAG_COUNTRYISO;
}

void ContactsCall::setVoicemailUri(string const& voicemailUri) noexcept
{
    this->voicemailUri = voicemailUri;
    this->validFlags |= ContactsCall::ATTRFLAG_VOICEMAILURI;
}

void ContactsCall::setIsRead(int16_t isRead) noexcept
{
    this->isRead = isRead;
    this->validFlags |= ContactsCall::ATTRFLAG_ISREAD;
}

void ContactsCall::setGeocodedLocation(string const& geocodedLocation) noexcept
{
    this->geocodedLocation = geocodedLocation;
    this->validFlags |= ContactsCall::ATTRFLAG_GEOCODEDLOCATION;
}

void ContactsCall::setLookupUri(string const& lookupUri) noexcept
{
    this->lookupUri = lookupUri;
    this->validFlags |= ContactsCall::ATTRFLAG_LOOKUPURI;
}

void ContactsCall::setMatchedNumber(string const& matchedNumber) noexcept
{
    this->matchedNumber = matchedNumber;
    this->validFlags |= ContactsCall::ATTRFLAG_MATCHEDNUMBER;
}

void ContactsCall::setNormalizedNumber(string const& normalizedNumber) noexcept
{
    this->normalizedNumber = normalizedNumber;
    this->validFlags |= ContactsCall::ATTRFLAG_NORMALIZEDNUMBER;
}

void ContactsCall::setPhotoId(int16_t photoId) noexcept
{
    this->photoId = photoId;
    this->validFlags |= ContactsCall::ATTRFLAG_PHOTOID;
}

void ContactsCall::setFormattedNumber(string const& formattedNumber) noexcept
{
    this->formattedNumber = formattedNumber;
    this->validFlags |= ContactsCall::ATTRFLAG_FORMATTEDNUMBER;
}

void ContactsCall::setData(string const& data) noexcept
{
    this->data = data;
    this->validFlags |= ContactsCall::ATTRFLAG_DATA;
}

void ContactsCall::setHasContent(int16_t hasContent) noexcept
{
    this->hasContent = hasContent;
    this->validFlags |= ContactsCall::ATTRFLAG_HASCONTENT;
}

void ContactsCall::setMimeType(string const& mimeType) noexcept
{
    this->mimeType = mimeType;
    this->validFlags |= ContactsCall::ATTRFLAG_MIMETYPE;
}

void ContactsCall::setSourceData(string const& sourceData) noexcept
{
    this->sourceData = sourceData;
    this->validFlags |= ContactsCall::ATTRFLAG_SOURCEDATA;
}

void ContactsCall::setSourcePackage(string const& sourcePackage) noexcept
{
    this->sourcePackage = sourcePackage;
    this->validFlags |= ContactsCall::ATTRFLAG_SOURCEPACKAGE;
}

void ContactsCall::setState(int16_t state) noexcept
{
    this->state = state;
    this->validFlags |= ContactsCall::ATTRFLAG_STATE;
}

void ContactsCall::setSubscription(int16_t subscription) noexcept
{
    this->subscription = subscription;
    this->validFlags |= ContactsCall::ATTRFLAG_SUBSCRIPTION;
}

void ContactsCall::setIsPrivate(int16_t isPrivate) noexcept
{
    this->isPrivate = isPrivate;
    this->validFlags |= ContactsCall::ATTRFLAG_ISPRIVATE;
}

void ContactsCall::setRingTimes(int16_t ringTimes) noexcept
{
    this->ringTimes = ringTimes;
    this->validFlags |= ContactsCall::ATTRFLAG_RINGTIMES;
}

void ContactsCall::setPageNumber(uint32_t pageNumber) noexcept
{
    this->pageNumber = pageNumber;
    this->validFlags |= ContactsCall::ATTRFLAG_PAGENUMBER;
}

int64_t ContactsCall::getId() const noexcept
{
    return this->id;
}

string ContactsCall::getNumber() const noexcept
{
    return this->number;
}

uint64_t ContactsCall::getDate() const noexcept
{
    return this->date;
}

int32_t ContactsCall::getDuration() const noexcept
{
    return this->duration;
}

int16_t ContactsCall::getType() const noexcept
{
    return this->type;
}

int16_t ContactsCall::getIsNew() const noexcept
{
    return this->isNew;
}

string ContactsCall::getName() const noexcept
{
    return this->name;
}

int16_t ContactsCall::getNumberType() const noexcept
{
    return this->numberType;
}

string ContactsCall::getNumberLabel() const noexcept
{
    return this->numberLabel;
}

string ContactsCall::getCountryIso() const noexcept
{
    return this->countryIso;
}

string ContactsCall::getVoicemailUri() const noexcept
{
    return this->voicemailUri;
}

int16_t ContactsCall::getIsRead() const noexcept
{
    return this->isRead;
}

string ContactsCall::getGeocodedLocation() const noexcept
{
    return this->geocodedLocation;
}

string ContactsCall::getLookupUri() const noexcept
{
    return this->lookupUri;
}

string ContactsCall::getMatchedNumber() const noexcept
{
    return this->matchedNumber;
}

string ContactsCall::getNormalizedNumber() const noexcept
{
    return this->normalizedNumber;
}

int16_t ContactsCall::getPhotoId() const noexcept
{
    return this->photoId;
}

string ContactsCall::getFormattedNumber() const noexcept
{
    return this->formattedNumber;
}

string ContactsCall::getData() const noexcept
{
    return this->data;
}

int16_t ContactsCall::getHasContent() const noexcept
{
    return this->hasContent;
}

string ContactsCall::getMimeType() const noexcept
{
    return this->mimeType;
}

string ContactsCall::getSourceData() const noexcept
{
    return this->sourceData;
}

string ContactsCall::getSourcePackage() const noexcept
{
    return this->sourcePackage;
}

int16_t ContactsCall::getState() const noexcept
{
    return this->state;
}

int16_t ContactsCall::getSubscription() const noexcept
{
    return this->subscription;
}

int16_t ContactsCall::getIsPrivate() const noexcept
{
    return this->isPrivate;
}

int16_t ContactsCall::getRingTimes() const noexcept
{
    return this->ringTimes;
}

uint32_t ContactsCall::getPageNumber() const noexcept
{
    return this->pageNumber;
}


shared_ptr<ContactsCall> ContactsCall::translate(hudun::sqlite::Record const& record) noexcept
{
    shared_ptr<ContactsCall> contactsCall(new ContactsCall(ContactsCall::OBTAINWAY_CELL_TYPE_SEEK));

    // id;
    const shared_ptr<FieldValue> idFieldValue = record.getFieldValueByFieldName("_id");
    if(idFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_ID);
    }
    else if(idFieldValue->isInteger())
    {
        contactsCall->setId(idFieldValue->getIntegerValue());
    }

    // number;
    const shared_ptr<FieldValue> numberFieldValue = record.getFieldValueByFieldName("number");
    if(numberFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_NUMBER);
    }
    else if(numberFieldValue->isText())
    {
        contactsCall->setNumber(numberFieldValue->getTextValue());

        // pageNumber;
        contactsCall->setPageNumber(numberFieldValue->getPageNumber());
    }

    // date;
    const shared_ptr<FieldValue> dateFieldValue = record.getFieldValueByFieldName("date");
    if(dateFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_DATE);
    }
    else if(dateFieldValue->isInteger())
    {
        contactsCall->setDate(dateFieldValue->getIntegerValue());
    }

    // duration;
    const shared_ptr<FieldValue> durationFieldValue = record.getFieldValueByFieldName("duration");
    if(durationFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_DURATION);
    }
    else if(durationFieldValue->isInteger())
    {
        contactsCall->setDuration(durationFieldValue->getIntegerValue());
    }

    // type;
    const shared_ptr<FieldValue> typeFieldValue = record.getFieldValueByFieldName("type");
    if(typeFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_TYPE);
    }
    else if(typeFieldValue->isInteger())
    {
        contactsCall->setType(typeFieldValue->getIntegerValue());
    }

    // isNew;
    const shared_ptr<FieldValue> isNewFieldValue = record.getFieldValueByFieldName("new");
    if(isNewFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_ISNEW);
    }
    else if(isNewFieldValue->isInteger())
    {
        contactsCall->setIsNew(isNewFieldValue->getIntegerValue());
    }

    // name;
    const shared_ptr<FieldValue> nameFieldValue = record.getFieldValueByFieldName("name");
    if(nameFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_NAME);
    }
    else if(nameFieldValue->isText())
    {
        contactsCall->setName(nameFieldValue->getTextValue());
    }

    // numberType;
    const shared_ptr<FieldValue> numberTypeFieldValue = record.getFieldValueByFieldName("numbertype");
    if(numberTypeFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_NUMBERTYPE);
    }
    else if(numberTypeFieldValue->isInteger())
    {
        contactsCall->setNumberType(numberTypeFieldValue->getIntegerValue());
    }

    // numberLabel;
    const shared_ptr<FieldValue> numberLabelFieldValue = record.getFieldValueByFieldName("numberLabel");
    if(numberLabelFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_NUMBERLABEL);
    }
    else if(numberLabelFieldValue->isText())
    {
        contactsCall->setNumberLabel(numberLabelFieldValue->getTextValue());
    }

    // countryIso;
    const shared_ptr<FieldValue> countryIsoFieldValue = record.getFieldValueByFieldName("countryiso");
    if(countryIsoFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_COUNTRYISO);
    }
    else if(countryIsoFieldValue->isText())
    {
        contactsCall->setCountryIso(countryIsoFieldValue->getTextValue());
    }

    // voicemailUri;
    const shared_ptr<FieldValue> voicemailUriFieldValue = record.getFieldValueByFieldName("voicemail_uri");
    if(voicemailUriFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_VOICEMAILURI);
    }
    else if(voicemailUriFieldValue->isText())
    {
        contactsCall->setVoicemailUri(voicemailUriFieldValue->getTextValue());
    }

    // isRead;
    const shared_ptr<FieldValue> isReadFieldValue = record.getFieldValueByFieldName("is_read");
    if(isReadFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_ISREAD);
    }
    else if(isReadFieldValue->isInteger())
    {
        contactsCall->setIsRead(isReadFieldValue->getIntegerValue());
    }

    // geocodedLocation;
    const shared_ptr<FieldValue> geocodedLocationFieldValue = record.getFieldValueByFieldName("geocoded_location");
    if(geocodedLocationFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_GEOCODEDLOCATION);
    }
    else if(geocodedLocationFieldValue->isText())
    {
        contactsCall->setGeocodedLocation(geocodedLocationFieldValue->getTextValue());
    }

    // lookupUri;
    const shared_ptr<FieldValue> lookupUriFieldValue = record.getFieldValueByFieldName("lookup_uri");
    if(lookupUriFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_LOOKUPURI);
    }
    else if(lookupUriFieldValue->isText())
    {
        contactsCall->setLookupUri(lookupUriFieldValue->getTextValue());
    }

    // matchedNumber;
    const shared_ptr<FieldValue> matchedNumberFieldValue = record.getFieldValueByFieldName("matched_number");
    if(matchedNumberFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_MATCHEDNUMBER);
    }
    else if(matchedNumberFieldValue->isText())
    {
        contactsCall->setMatchedNumber(matchedNumberFieldValue->getTextValue());
    }

    // normalizedNumber;
    const shared_ptr<FieldValue> normalizedNumberFieldValue = record.getFieldValueByFieldName("normalized_number");
    if(normalizedNumberFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_NORMALIZEDNUMBER);
    }
    else if(normalizedNumberFieldValue->isText())
    {
        contactsCall->setNormalizedNumber(normalizedNumberFieldValue->getTextValue());
    }

    // photoId;
    const shared_ptr<FieldValue> photoIdFieldValue = record.getFieldValueByFieldName("photo_id");
    if(photoIdFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_PHOTOID);
    }
    else if(photoIdFieldValue->isInteger())
    {
        contactsCall->setPhotoId(photoIdFieldValue->getIntegerValue());
    }

    // formattedNumber;
    const shared_ptr<FieldValue> formattedNumberFieldValue = record.getFieldValueByFieldName("formatted_number");
    if(formattedNumberFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_FORMATTEDNUMBER);
    }
    else if(formattedNumberFieldValue->isText())
    {
        contactsCall->setFormattedNumber(formattedNumberFieldValue->getTextValue());
    }

    // data;
    const shared_ptr<FieldValue> dataFieldValue = record.getFieldValueByFieldName("_data");
    if(dataFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_DATA);
    }
    else if(dataFieldValue->isText())
    {
        contactsCall->setData(dataFieldValue->getTextValue());
    }

    // hasContent;
    const shared_ptr<FieldValue> hasContentFieldValue = record.getFieldValueByFieldName("has_content");
    if(hasContentFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_HASCONTENT);
    }
    else if(hasContentFieldValue->isInteger())
    {
        contactsCall->setHasContent(hasContentFieldValue->getIntegerValue());
    }

    // mimeType;
    const shared_ptr<FieldValue> mimeTypeFieldValue = record.getFieldValueByFieldName("mime_type");
    if(mimeTypeFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_MIMETYPE);
    }
    else if(mimeTypeFieldValue->isText())
    {
        contactsCall->setMimeType(mimeTypeFieldValue->getTextValue());
    }

    // sourceData;
    const shared_ptr<FieldValue> sourceDataFieldValue = record.getFieldValueByFieldName("source_data");
    if(sourceDataFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_SOURCEDATA);
    }
    else if(sourceDataFieldValue->isText())
    {
        contactsCall->setSourceData(sourceDataFieldValue->getTextValue());
    }

    // sourcePackage;
    const shared_ptr<FieldValue> sourcePackageFieldValue = record.getFieldValueByFieldName("source_package");
    if(sourcePackageFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_SOURCEPACKAGE);
    }
    else if(sourcePackageFieldValue->isText())
    {
        contactsCall->setSourcePackage(sourcePackageFieldValue->getTextValue());
    }

    // state;
    const shared_ptr<FieldValue> stateFieldValue = record.getFieldValueByFieldName("state");
    if(stateFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_STATE);
    }
    else if(stateFieldValue->isInteger())
    {
        contactsCall->setState(stateFieldValue->getIntegerValue());
    }

    // subscription;
    const shared_ptr<FieldValue> subscriptionFieldValue = record.getFieldValueByFieldName("subscription");
    if(subscriptionFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_SUBSCRIPTION);
    }
    else if(subscriptionFieldValue->isInteger())
    {
        contactsCall->setSubscription(subscriptionFieldValue->getIntegerValue());
    }

    // isPrivate;
    const shared_ptr<FieldValue> isPrivateFieldValue = record.getFieldValueByFieldName("is_private");
    if(isPrivateFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_ISPRIVATE);
    }
    else if(isPrivateFieldValue->isInteger())
    {
        contactsCall->setIsPrivate(isPrivateFieldValue->getIntegerValue());
    }

    // ringTimes;
    const shared_ptr<FieldValue> ringTimesFieldValue = record.getFieldValueByFieldName("ring_times");
    if(ringTimesFieldValue == nullptr)
    {
        contactsCall->setNullValue(ContactsCall::ATTRFLAG_RINGTIMES);
    }
    else if(ringTimesFieldValue->isInteger())
    {
        contactsCall->setRingTimes(ringTimesFieldValue->getIntegerValue());
    }

    return contactsCall;
}

ostream& operator<<(ostream& out, ContactsCall const& contactsCall) noexcept
{
    // pageNumber;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_PAGENUMBER) && !contactsCall.isNull(ContactsCall::ATTRFLAG_PAGENUMBER))
    {
        out << "pageNumber: " << contactsCall.getPageNumber() << endl;
    }

    // id;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_ID) && !contactsCall.isNull(ContactsCall::ATTRFLAG_ID))
    {
        out << "_id: " << contactsCall.getId() << endl;
    }

    // number;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_NUMBER) && !contactsCall.isNull(ContactsCall::ATTRFLAG_NUMBER))
    {
        out << "number: " << contactsCall.getNumber() << endl;
    }

    // date;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_DATE) && !contactsCall.isNull(ContactsCall::ATTRFLAG_DATE))
    {
        out << "date: " << contactsCall.getDate() << endl;
    }

    // duration;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_DURATION) && !contactsCall.isNull(ContactsCall::ATTRFLAG_DURATION))
    {
        out << "duration: " << contactsCall.getDuration() << endl;
    }

    // type;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_TYPE) && !contactsCall.isNull(ContactsCall::ATTRFLAG_TYPE))
    {
        out << "type: " << contactsCall.getType() << endl;
    }

    // isNew;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_ISNEW) && !contactsCall.isNull(ContactsCall::ATTRFLAG_ISNEW))
    {
        out << "new: " << contactsCall.getIsNew() << endl;
    }

    // name;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_NAME) && !contactsCall.isNull(ContactsCall::ATTRFLAG_NAME))
    {
        out << "name: " << contactsCall.getName() << endl;
    }

    // numberType;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_NUMBERTYPE) && !contactsCall.isNull(ContactsCall::ATTRFLAG_NUMBERTYPE))
    {
        out << "numbertype: " << contactsCall.getNumberType() << endl;
    }

    // numberLabel;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_NUMBERLABEL) && !contactsCall.isNull(ContactsCall::ATTRFLAG_NUMBERLABEL))
    {
        out << "numberlabel: " << contactsCall.getNumberLabel() << endl;
    }

    // countryIso;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_COUNTRYISO) && !contactsCall.isNull(ContactsCall::ATTRFLAG_COUNTRYISO))
    {
        out << "countryiso: " << contactsCall.getCountryIso() << endl;
    }

    // voicemailUri;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_VOICEMAILURI) && !contactsCall.isNull(ContactsCall::ATTRFLAG_VOICEMAILURI))
    {
        out << "voicemail_uri: " << contactsCall.getVoicemailUri() << endl;
    }

    // isRead;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_ISREAD) && !contactsCall.isNull(ContactsCall::ATTRFLAG_ISREAD))
    {
        out << "is_read: " << contactsCall.getIsRead() << endl;
    }

    // geocodedLocation;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_GEOCODEDLOCATION) && !contactsCall.isNull(ContactsCall::ATTRFLAG_GEOCODEDLOCATION))
    {
        out << "geocoded_location: " << contactsCall.getGeocodedLocation() << endl;
    }

    // lookupUri;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_LOOKUPURI) && !contactsCall.isNull(ContactsCall::ATTRFLAG_LOOKUPURI))
    {
        out << "lookup_uri: " << contactsCall.getLookupUri() << endl;
    }

    // matchedNumber;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_MATCHEDNUMBER) && !contactsCall.isNull(ContactsCall::ATTRFLAG_MATCHEDNUMBER))
    {
        out << "matched_number: " << contactsCall.getMatchedNumber() << endl;
    }

    // normalizedNumber;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_NORMALIZEDNUMBER) && !contactsCall.isNull(ContactsCall::ATTRFLAG_NORMALIZEDNUMBER))
    {
        out << "normalized_number: " << contactsCall.getNormalizedNumber() << endl;
    }

    // photoId;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_PHOTOID) && !contactsCall.isNull(ContactsCall::ATTRFLAG_PHOTOID))
    {
        out << "photo_id: " << contactsCall.getPhotoId() << endl;
    }

    // formattedNumber;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_FORMATTEDNUMBER) && !contactsCall.isNull(ContactsCall::ATTRFLAG_FORMATTEDNUMBER))
    {
        out << "formatted_number: " << contactsCall.getFormattedNumber() << endl;
    }

    // data;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_DATA) && !contactsCall.isNull(ContactsCall::ATTRFLAG_DATA))
    {
        out << "_data: " << contactsCall.getData() << endl;
    }

    // hasContent;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_HASCONTENT) && !contactsCall.isNull(ContactsCall::ATTRFLAG_HASCONTENT))
    {
        out << "has_content: " << contactsCall.getHasContent() << endl;
    }

    // mimeType;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_MIMETYPE) && !contactsCall.isNull(ContactsCall::ATTRFLAG_MIMETYPE))
    {
        out << "mime_type: " << contactsCall.getMimeType() << endl;
    }

    // sourceData;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_SOURCEDATA) && !contactsCall.isNull(ContactsCall::ATTRFLAG_SOURCEDATA))
    {
        out << "source_data: " << contactsCall.getSourceData() << endl;
    }

    // sourcePackage;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_SOURCEPACKAGE) && !contactsCall.isNull(ContactsCall::ATTRFLAG_SOURCEPACKAGE))
    {
        out << "source_package: " << contactsCall.getSourcePackage() << endl;
    }

    // state;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_STATE) && !contactsCall.isNull(ContactsCall::ATTRFLAG_STATE))
    {
        out << "state: " << contactsCall.getState() << endl;
    }

    // subscription;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_SUBSCRIPTION) && !contactsCall.isNull(ContactsCall::ATTRFLAG_SUBSCRIPTION))
    {
        out << "subscription: " << contactsCall.getSubscription() << endl;
    }

    // isPrivate;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_ISPRIVATE) && !contactsCall.isNull(ContactsCall::ATTRFLAG_ISPRIVATE))
    {
        out << "is_private: " << contactsCall.getIsPrivate() << endl;
    }

    // ringTimes;
    if(contactsCall.isValid(ContactsCall::ATTRFLAG_RINGTIMES) && !contactsCall.isNull(ContactsCall::ATTRFLAG_RINGTIMES))
    {
        out << "ring_times: " << contactsCall.getRingTimes() << endl;
    }

    return out;
}

