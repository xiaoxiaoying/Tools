#include <memory>
#include <regex>
#include <string>
#include <vector>

using namespace std;

#include "../include/Incident.h"

using namespace hudun::common;

#include <android/log.h>

#define TAG "scanning"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

#include "../include/Database.h"
#include "../include/RecordFilter.h"
#include "../include/SeekEventHandler.h"
#include "../include/SeekProgressIndicator.h"
#include "../include/SqliteDatabaseSeeker.h"

using namespace hudun::sqlite;

#include "../include/Table.h"

using namespace hudun::sqlite::schema;

#include "../include/SqliteAssistant.h"
#include "../include/ResultSet.h"

using namespace hudun::sqlite::baseapi;

#include "../include/ContactsCall.h"
#include "../include/ContactsCallSeeker.h"

using namespace hudun::sqlite::contacts;

// ͨ����¼��������
class NativeContactsCallRecordFilter : public RecordFilter {
public:
    bool check(shared_ptr<Record> const &record) const noexcept override {
        const static regex PHONE_NUMBER_REGEX("^\\+?[0-9]+$");
        const shared_ptr<FieldValue> numberFieldValue = record->getFieldValueByFieldName("number");
        if ((numberFieldValue == nullptr) || numberFieldValue->isNull() ||
            !numberFieldValue->isText()) {
            return false;
        }

        return regex_match(numberFieldValue->getTextValue(), PHONE_NUMBER_REGEX);
    }
};

// ͨ����¼�����¼���������
class NativeContactsCallSeekEventHandler : public SeekEventHandler {
private:
    vector<shared_ptr<ContactsCall>> contactsCalls;

public:

    void beforeSeekPage(uint32_t pageNumber) noexcept { };

    void afterSeekPage(uint32_t pageNumber) noexcept { };

    int seekedRecord(shared_ptr<Record> &record) noexcept override {
        shared_ptr<ContactsCall> contactsCall = ContactsCall::translate(*record);
        this->contactsCalls.push_back(contactsCall);
        return 1;
    }

    const vector<shared_ptr<ContactsCall>> &getContactsCalls() const noexcept {
        return this->contactsCalls;
    }

    void reset() noexcept {
        this->contactsCalls.clear();
    }
};

void ContactsCallSeeker::parseCallsTable(Incident &incident) {
    // ��SqliteApi��Դ���ݿ⣻
    void *sourceDb = SqliteAssistant::openDatabase(this->sourceDbFilename, incident);
    if (!incident.succeeded()) return;

    // ��ȡ���ű��DDL;
    ResultSet resultSet;
    SqliteAssistant::query(sourceDb, 1,
                           "SELECT sql FROM sqlite_master WHERE (type='table') AND (tbl_name = 'calls');",
                           resultSet, incident);
    if (!incident.succeeded()) return;
    long num = resultSet.getRowsNum();
    LOGI("number = %ld", num);
    if (num == 0) {
        incident.set(Incident::FAIL_GENERAL, "table[calls] not exist!", __FILE__, __func__,
                     __LINE__);
        return;
    }

    shared_ptr<ColValue> ddlColValue = resultSet.getValue(0, 0);
    string ddl = ddlColValue->getTextValue();
    LOGI("call ddl = %s", ddl.c_str());
    // ����DDL;
    this->callsTable = Table::parse(ddl);
    if (!this->callsTable.check()) {
        incident.set(Incident::FAIL_GENERAL, "Check 'calls' table fail!", __FILE__, __func__,
                     __LINE__);
        return;
    }

    // �ر���SqliteApi�򿪵�Դ���ݿ⣻
    SqliteAssistant::closeDatabase(sourceDb);
}

void ContactsCallSeeker::openDatabase(Incident &incident) noexcept {
    this->database.open(this->sourceDbFilename, incident);
}

void ContactsCallSeeker::readDatabaseData(Incident &incident) noexcept {
    this->databaseData = this->database.getData(incident);
}

ContactsCallSeeker::ContactsCallSeeker(string const &sourceDbFilename) noexcept {
    this->sourceDbFilename = sourceDbFilename;
    this->seekProgressIndicator = nullptr;
}

ContactsCallSeeker::~ContactsCallSeeker() {
    this->reset();
}

void ContactsCallSeeker::registerSeekProgressIndicator(
        SeekProgressIndicator *seekProgressIndicator) noexcept {
    this->seekProgressIndicator = seekProgressIndicator;
}

void ContactsCallSeeker::seek(uint32_t threadNum, Incident &incident) noexcept {
    // �������ű�ṹ��
    this->parseCallsTable(incident);
    if (!incident.succeeded()) {
        LOGI("====================");
        return;
    }

    // �����ݿ⣻
    this->openDatabase(incident);
    if (!incident.succeeded()) return;

    // ��ȡ���ݿ����ݣ�
    this->readDatabaseData(incident);
    if (!incident.succeeded()) return;

    // ��������ͨ����¼������
    NativeContactsCallRecordFilter nativeContactsCallRecordFilter;

    // ��������ͨ����¼�¼���������
    SeekEventHandler *callsSeekEventHandlers[threadNum];
    for (uint32_t i = 0; i < threadNum; i++) {
        callsSeekEventHandlers[i] = new NativeContactsCallSeekEventHandler();
    }

    // ������������
    SqliteDatabaseSeeker callsSqliteDatabaseSeeker(this->database, this->callsTable,
                                                   &nativeContactsCallRecordFilter, false);
    callsSqliteDatabaseSeeker.seek(threadNum, this->seekProgressIndicator, callsSeekEventHandlers,
                                   incident);
    if (!incident.succeeded()) return;

    // �ϲ����������
    for (uint32_t i = 0; i < threadNum; i++) {
        NativeContactsCallSeekEventHandler *callsSeekEventHandler = (NativeContactsCallSeekEventHandler *) callsSeekEventHandlers[i];
        vector<shared_ptr<ContactsCall>> _contactsCalls = callsSeekEventHandler->getContactsCalls();
        this->contactsCalls.insert(this->contactsCalls.end(), _contactsCalls.begin(),
                                   _contactsCalls.end());
        delete callsSeekEventHandler;
    }

    return;
}

vector<shared_ptr<ContactsCall>> ContactsCallSeeker::getContactsCalls() const noexcept {
    return this->contactsCalls;
}

void ContactsCallSeeker::reset() noexcept {
    this->database.close();
    this->contactsCalls.clear();
}

