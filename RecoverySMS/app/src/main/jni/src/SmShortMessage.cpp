#include <iostream>
#include <string>

using namespace std;

#include <android/log.h>

#define TAG "scanning"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

#include "../include/StringFacility.h"

using namespace hudun::common;

#include "../include/Entity.h"
#include "../include/FieldValue.h"
#include "../include/Record.h"

using namespace hudun::sqlite;

#include "../include/SmShortMessage.h"

using namespace hudun::sqlite::sm;


SmShortMessage::SmShortMessage(uint16_t obtainWay) noexcept: Entity(obtainWay) {
}

void SmShortMessage::setSmId(int64_t smId) noexcept {
    this->smId = smId;
    this->validFlags |= SmShortMessage::ATTRFLAG_SMID;
}

void SmShortMessage::setThreadId(int threadId) noexcept {
    this->threadId = threadId;
    this->validFlags |= SmShortMessage::ATTRFLAG_THREADID;
}

void SmShortMessage::setAddress(string const &address) noexcept {
    LOGI("address = %s", address.c_str());
    unsigned long index = address.find("\u0001");
    LOGI("index = %ld", index);
    if (string::npos != index) {
        this->address = address.substr(0, index);
        LOGI("address = %s", this->address.c_str());
    }
    else
        this->address = address;

    this->validFlags |= SmShortMessage::ATTRFLAG_ADDRESS;
}

void SmShortMessage::setMSize(int mSize) noexcept {
    this->mSize = mSize;
    this->validFlags |= SmShortMessage::ATTRFLAG_MSIZE;
}

void SmShortMessage::setPerson(int person) noexcept {
    this->person = person;
    this->validFlags |= SmShortMessage::ATTRFLAG_PERSON;
}

void SmShortMessage::setDate(int64_t date) noexcept {
    this->date = date;
    this->validFlags |= SmShortMessage::ATTRFLAG_DATE;
}

void SmShortMessage::setDateSent(int64_t dateSent) noexcept {
    this->dateSent = dateSent;
    this->validFlags |= SmShortMessage::ATTRFLAG_DATESENT;
}

void SmShortMessage::setProtocol(int protocol) noexcept {
    this->protocol = protocol;
    this->validFlags |= SmShortMessage::ATTRFLAG_PROTOCOL;
}

void SmShortMessage::setRead(int read) noexcept {
    this->read = read;
    this->validFlags |= SmShortMessage::ATTRFLAG_READ;
}

void SmShortMessage::setStatus(int status) noexcept {
    this->status = status;
    this->validFlags |= SmShortMessage::ATTRFLAG_STATUS;
}

void SmShortMessage::setType(int type) noexcept {
    this->type = type;
    this->validFlags |= SmShortMessage::ATTRFLAG_TYPE;
}

void SmShortMessage::setReplyPathPresent(int replyPathPresent) noexcept {
    this->replyPathPresent = replyPathPresent;
    this->validFlags |= SmShortMessage::ATTRFLAG_REPLYPATHPRESENT;
}

void SmShortMessage::setSubject(string const &subject) noexcept {
    this->subject = subject;
    this->validFlags |= SmShortMessage::ATTRFLAG_SUBJECT;
}

void SmShortMessage::setBody(string const &body) noexcept {
    this->body = body;
    this->validFlags |= SmShortMessage::ATTRFLAG_BODY;
}

void SmShortMessage::setServiceCenter(string const &serviceCenter) noexcept {
    this->serviceCenter = serviceCenter;
    this->validFlags |= SmShortMessage::ATTRFLAG_SERVICECENTER;
}

void SmShortMessage::setLocked(int locked) noexcept {
    this->locked = locked;
    this->validFlags |= SmShortMessage::ATTRFLAG_LOCKED;
}

void SmShortMessage::setSubId(int64_t subId) noexcept {
    this->subId = subId;
    this->validFlags |= SmShortMessage::ATTRFLAG_SUBID;
}

void SmShortMessage::setReferenceNumber(int referenceNumber) noexcept {
    this->referenceNumber = referenceNumber;
    this->validFlags |= SmShortMessage::ATTRFLAG_REFERENCENUMBER;
}

void SmShortMessage::setErrorCode(int errorCode) noexcept {
    this->errorCode = errorCode;
    this->validFlags |= SmShortMessage::ATTRFLAG_ERRORCODE;
}

void SmShortMessage::setSeen(int seen) noexcept {
    this->seen = seen;
    this->validFlags |= SmShortMessage::ATTRFLAG_SEEN;
}

void SmShortMessage::setPri(int pri) noexcept {
    this->pri = pri;
    this->validFlags |= SmShortMessage::ATTRFLAG_PRI;
}

void SmShortMessage::setImsi(string const &imsi) noexcept {
    this->imsi = imsi;
    this->validFlags |= SmShortMessage::ATTRFLAG_IMSI;
}

void SmShortMessage::setPageNumber(uint32_t pageNumber) noexcept {
    this->pageNumber = pageNumber;
    this->validFlags |= SmShortMessage::ATTRFLAG_PAGENUMBER;
}

int64_t SmShortMessage::getSmId() const noexcept {
    return this->smId;
}

int SmShortMessage::getThreadId() const noexcept {
    return this->threadId;
}

string SmShortMessage::getAddress() const noexcept {
    return this->address;
}

int SmShortMessage::getMSize() const noexcept {
    return this->mSize;
}

int SmShortMessage::getPerson() const noexcept {
    return this->person;
}

int64_t SmShortMessage::getDate() const noexcept {
    return this->date;
}

int64_t SmShortMessage::getDateSent() const noexcept {
    return this->dateSent;
}

int SmShortMessage::getProtocol() const noexcept {
    return this->protocol;
}

int SmShortMessage::getRead() const noexcept {
    return this->read;
}

int SmShortMessage::getStatus() const noexcept {
    return this->status;
}

int SmShortMessage::getType() const noexcept {
    return this->type;
}

int SmShortMessage::getReplyPathPresent() const noexcept {
    return this->replyPathPresent;
}

string SmShortMessage::getSubject() const noexcept {
    return this->subject;
}

string SmShortMessage::getBody() const noexcept {
    return this->body;
}

string SmShortMessage::getServiceCenter() const noexcept {
    return this->serviceCenter;
}

int SmShortMessage::getLocked() const noexcept {
    return this->locked;
}

int64_t SmShortMessage::getSubId() const noexcept {
    return this->subId;
}

int SmShortMessage::getReferenceNumber() const noexcept {
    return this->referenceNumber;
}

int SmShortMessage::getErrorCode() const noexcept {
    return this->errorCode;
}

int SmShortMessage::getSeen() const noexcept {
    return this->seen;
}

int SmShortMessage::getPri() const noexcept {
    return this->pri;
}

string SmShortMessage::getImsi() const noexcept {
    return this->imsi;
}

uint32_t SmShortMessage::getPageNumber() const noexcept {
    return this->pageNumber;
}

shared_ptr<SmShortMessage> SmShortMessage::translate(Record const &record) noexcept {
    shared_ptr<SmShortMessage> smShortMessage(
            new SmShortMessage(SmShortMessage::OBTAINWAY_CELL_TYPE_SEEK));

    // smId;
    const shared_ptr<FieldValue> smIdFieldValue = record.getFieldValueByFieldName("smId");
    if (smIdFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_SMID);
    }
    else if (smIdFieldValue->isInteger()) {
        smShortMessage->setSmId(smIdFieldValue->getIntegerValue());
    }

    // threadId;
    const shared_ptr<FieldValue> threadIdFieldValue = record.getFieldValueByFieldName("threadId");
    if (threadIdFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_THREADID);
    }
    else if (threadIdFieldValue->isInteger()) {
        smShortMessage->setThreadId(threadIdFieldValue->getIntegerValue());
    }

    // address;
    const shared_ptr<FieldValue> addressFieldValue = record.getFieldValueByFieldName("address");
    if (addressFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_ADDRESS);
    }
    else if (addressFieldValue->isText()) {
        smShortMessage->setAddress(addressFieldValue->getTextValue());
    }

    // mSize;
    const shared_ptr<FieldValue> mSizeFieldValue = record.getFieldValueByFieldName("mSize");
    if (mSizeFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_MSIZE);
    }
    else if (mSizeFieldValue->isInteger()) {
        smShortMessage->setMSize(mSizeFieldValue->getIntegerValue());
    }

    // person;
    const shared_ptr<FieldValue> personFieldValue = record.getFieldValueByFieldName("person");
    if (personFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_PERSON);
    }
    else if (personFieldValue->isInteger()) {
        smShortMessage->setPerson(personFieldValue->getIntegerValue());
    }

    // date;
    const shared_ptr<FieldValue> dateFieldValue = record.getFieldValueByFieldName("date");
    if (dateFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_DATE);
    }
    else if (dateFieldValue->isInteger()) {
        smShortMessage->setDate(dateFieldValue->getIntegerValue());
    }

    // dateSent;
    const shared_ptr<FieldValue> dateSentFieldValue = record.getFieldValueByFieldName("dateSent");
    if (dateSentFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_DATESENT);
    }
    else if (dateSentFieldValue->isInteger()) {
        smShortMessage->setDateSent(dateSentFieldValue->getIntegerValue());
    }

    // protocol;
    const shared_ptr<FieldValue> protocolFieldValue = record.getFieldValueByFieldName("protocol");
    if (protocolFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_PROTOCOL);
    }
    else if (protocolFieldValue->isInteger()) {
        smShortMessage->setProtocol(protocolFieldValue->getIntegerValue());
    }

    // read;
    const shared_ptr<FieldValue> readFieldValue = record.getFieldValueByFieldName("read");
    if (readFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_READ);
    }
    else if (readFieldValue->isInteger()) {
        smShortMessage->setRead(readFieldValue->getIntegerValue());
    }

    // status;
    const shared_ptr<FieldValue> statusFieldValue = record.getFieldValueByFieldName("status");
    if (statusFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_STATUS);
    }
    else if (statusFieldValue->isInteger()) {
        smShortMessage->setStatus(statusFieldValue->getIntegerValue());
    }

    // type;
    const shared_ptr<FieldValue> typeFieldValue = record.getFieldValueByFieldName("type");
    if (typeFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_TYPE);
    }
    else if (typeFieldValue->isInteger()) {
        smShortMessage->setType(typeFieldValue->getIntegerValue());
    }

    // replyPathPresent;
    const shared_ptr<FieldValue> replyPathPresentFieldValue = record.getFieldValueByFieldName(
            "replyPathPresent");
    if (replyPathPresentFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_REPLYPATHPRESENT);
    }
    else if (replyPathPresentFieldValue->isInteger()) {
        smShortMessage->setReplyPathPresent(replyPathPresentFieldValue->getIntegerValue());
    }

    // subject;
    const shared_ptr<FieldValue> subjectFieldValue = record.getFieldValueByFieldName("subject");
    if (subjectFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_SUBJECT);
    }
    else if (subjectFieldValue->isText()) {
        smShortMessage->setSubject(subjectFieldValue->getTextValue());
    }

    // body;
    const shared_ptr<FieldValue> bodyFieldValue = record.getFieldValueByFieldName("body");
    if (bodyFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_BODY);
    }
    else if (bodyFieldValue->isText()) {
        smShortMessage->setBody(StringFacility::washUtf8(bodyFieldValue->getTextValue()));

        // pageNumber;
        smShortMessage->setPageNumber(bodyFieldValue->getPageNumber());
    }

    // serviceCenter;
    const shared_ptr<FieldValue> serviceCenterFieldValue = record.getFieldValueByFieldName(
            "serviceCenter");
    if (serviceCenterFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_SERVICECENTER);
    }
    else if (serviceCenterFieldValue->isText()) {
        smShortMessage->setServiceCenter(serviceCenterFieldValue->getTextValue());
    }

    // locked;
    const shared_ptr<FieldValue> lockedFieldValue = record.getFieldValueByFieldName("locked");
    if (lockedFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_LOCKED);
    }
    else if (lockedFieldValue->isInteger()) {
        smShortMessage->setLocked(lockedFieldValue->getIntegerValue());
    }

    // subId;
    const shared_ptr<FieldValue> subIdFieldValue = record.getFieldValueByFieldName("subId");
    if (subIdFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_SUBID);
    }
    else if (subIdFieldValue->isInteger()) {
        smShortMessage->setSubId(subIdFieldValue->getIntegerValue());
    }

    // referenceNumber;
    const shared_ptr<FieldValue> referenceNumberFieldValue = record.getFieldValueByFieldName(
            "referenceNumber");
    if (referenceNumberFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_REFERENCENUMBER);
    }
    else if (referenceNumberFieldValue->isInteger()) {
        smShortMessage->setReferenceNumber(referenceNumberFieldValue->getIntegerValue());
    }

    // errorCode;
    const shared_ptr<FieldValue> errorCodeFieldValue = record.getFieldValueByFieldName("errorCode");
    if (errorCodeFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_ERRORCODE);
    }
    else if (errorCodeFieldValue->isInteger()) {
        smShortMessage->setErrorCode(errorCodeFieldValue->getIntegerValue());
    }

    // seen;
    const shared_ptr<FieldValue> seenFieldValue = record.getFieldValueByFieldName("seen");
    if (seenFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_SEEN);
    }
    else if (seenFieldValue->isInteger()) {
        smShortMessage->setSeen(seenFieldValue->getIntegerValue());
    }

    // pri;
    const shared_ptr<FieldValue> priFieldValue = record.getFieldValueByFieldName("pri");
    if (priFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_PRI);
    }
    else if (priFieldValue->isInteger()) {
        smShortMessage->setPri(priFieldValue->getIntegerValue());
    }

    // imsi;
    const shared_ptr<FieldValue> imsiFieldValue = record.getFieldValueByFieldName("imsi");
    if (imsiFieldValue == nullptr) {
        smShortMessage->setNullValue(SmShortMessage::ATTRFLAG_IMSI);
    }
    else if (imsiFieldValue->isText()) {
        smShortMessage->setImsi(imsiFieldValue->getTextValue());
    }

    return smShortMessage;
}

ostream &operator<<(ostream &out, SmShortMessage const &smShortMessage) noexcept {
    // smId;
    if (smShortMessage.isValid(SmShortMessage::ATTRFLAG_SMID) &&
        !smShortMessage.isNull(SmShortMessage::ATTRFLAG_SMID)) {
        out << "smId: " << smShortMessage.getSmId() << endl;
    }

    // address;
    if (smShortMessage.isValid(SmShortMessage::ATTRFLAG_ADDRESS) &&
        !smShortMessage.isNull(SmShortMessage::ATTRFLAG_ADDRESS)) {
        out << "address: " << smShortMessage.getAddress() << endl;
    }

    // date;
    if (smShortMessage.isValid(SmShortMessage::ATTRFLAG_DATE) &&
        !smShortMessage.isNull(SmShortMessage::ATTRFLAG_DATE)) {
        out << "date: " << smShortMessage.getDate() << endl;
    }

    // dateSent;
    if (smShortMessage.isValid(SmShortMessage::ATTRFLAG_DATESENT) &&
        !smShortMessage.isNull(SmShortMessage::ATTRFLAG_DATESENT)) {
        out << "dateSent: " << smShortMessage.getDateSent() << endl;
    }

    // type;
    if (smShortMessage.isValid(SmShortMessage::ATTRFLAG_TYPE) &&
        !smShortMessage.isNull(SmShortMessage::ATTRFLAG_TYPE)) {
        out << "type: " << smShortMessage.getType() << endl;
    }

    // subject;
    if (smShortMessage.isValid(SmShortMessage::ATTRFLAG_SUBJECT) &&
        !smShortMessage.isNull(SmShortMessage::ATTRFLAG_SUBJECT)) {
        out << "subject: " << StringFacility::washUtf8(smShortMessage.getSubject()) << endl;
    }

    // body;
    if (smShortMessage.isValid(SmShortMessage::ATTRFLAG_BODY) &&
        !smShortMessage.isNull(SmShortMessage::ATTRFLAG_BODY)) {
        out << "body: " << StringFacility::washUtf8(smShortMessage.getBody()) << endl;
    }

    // pageNumber;
    if (smShortMessage.isValid(SmShortMessage::ATTRFLAG_PAGENUMBER) &&
        !smShortMessage.isNull(SmShortMessage::ATTRFLAG_PAGENUMBER)) {
        out << "pageNumber: " << smShortMessage.getPageNumber() << endl;
    }

    return out;
}
