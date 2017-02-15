#include <memory>
#include <string>
#include <vector>

using namespace std;

#include "../include/Incident.h"

using namespace hudun::common;

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

#include "../include/SmShortMessage.h"
#include "../include/SmShortMessageSeeker.h"

using namespace hudun::sqlite::sm;

#include <android/log.h>

#define TAG "scanning"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

// ���Ź�������
class NativeSmRecordFilter : public RecordFilter {
public:
    bool check(shared_ptr<Record> const &record) const noexcept override {

        const shared_ptr<FieldValue> addressFieldValue = record->getFieldValueByFieldName(
                "address");
        if ((addressFieldValue == nullptr) || addressFieldValue->isNull()) {
            return false;
        }

        const shared_ptr<FieldValue> bodyFieldValue = record->getFieldValueByFieldName("body");
        if ((bodyFieldValue == nullptr) || bodyFieldValue->isNull()) {
            return false;
        }

        return true;
    }
};

// ���������¼���������
class NativeSmSeekEventHandler : public hudun::sqlite::SeekEventHandler {
private:
    vector<shared_ptr<SmShortMessage>> smShortMessages;

public:

    void beforeSeekPage(uint32_t pageNumber) noexcept { };

    void afterSeekPage(uint32_t pageNumber) noexcept { };

    int seekedRecord(shared_ptr<Record> &record) noexcept override {
        shared_ptr<SmShortMessage> smShortMessage = SmShortMessage::translate(*record);
        this->smShortMessages.push_back(smShortMessage);
        return 1;
    }

    const vector<shared_ptr<SmShortMessage>> &getSmShortMessages() const noexcept {
        return this->smShortMessages;
    }

    void reset() noexcept {
        this->smShortMessages.clear();
    }
};

void SmShortMessageSeeker::parseSmsTable(Incident &incident) {
    // ��SqliteApi��Դ���ݿ⣻
    void *sourceDb = SqliteAssistant::openDatabase(this->sourceDbFilename, incident);
    if (!incident.succeeded()) return;

    // ��ȡ���ű��DDL;
    ResultSet resultSet;
    SqliteAssistant::query(sourceDb, 1,
                           "SELECT sql FROM sqlite_master WHERE (type='table') AND (tbl_name = 'sms');",
                           resultSet, incident);
    if (!incident.succeeded()) return;
    LOGI("result set rows Num %d", resultSet.getRowsNum());
    if (resultSet.getRowsNum() == 0) {
        incident.set(Incident::FAIL_GENERAL, "table[sms] not exist!", __FILE__, __func__, __LINE__);
        return;
    }

    shared_ptr<ColValue> ddlColValue = resultSet.getValue(0, 0);
    string ddl = ddlColValue->getTextValue();
    LOGI(" ddl %s", ddl.c_str());

    // ����DDL;
    this->smsTable = Table::parse(ddl);
    LOGI("table name = %s", smsTable.getTableName().c_str());

    if (!this->smsTable.check()) {
        incident.set(Incident::FAIL_GENERAL, "Check smsTable fail!", __FILE__, __func__, __LINE__);
        return;
    }
    LOGI("database close ");
    // �ر���SqliteApi�򿪵�Դ���ݿ⣻
    SqliteAssistant::closeDatabase(sourceDb);
}

void SmShortMessageSeeker::openDatabase(Incident &incident) noexcept {
    this->database.open(this->sourceDbFilename, incident);
}

void SmShortMessageSeeker::readDatabaseData(Incident &incident) noexcept {
    this->databaseData = this->database.getData(incident);
}

SmShortMessageSeeker::SmShortMessageSeeker(string const &sourceDbFilename) noexcept {
    this->sourceDbFilename = sourceDbFilename;
    this->seekProgressIndicator = nullptr;
}

SmShortMessageSeeker::~SmShortMessageSeeker() {
    this->reset();
}

void SmShortMessageSeeker::registerSeekProgressIndicator(
        SeekProgressIndicator *seekProgressIndicator) noexcept {
    this->seekProgressIndicator = seekProgressIndicator;
}

void SmShortMessageSeeker::seek(uint32_t threadNum, Incident &incident) noexcept {
    // �������ű�ṹ��
    this->parseSmsTable(incident);
    if (!incident.succeeded()) return;

    // �����ݿ⣻
    this->openDatabase(incident);
    if (!incident.succeeded()) return;

    // ��ȡ���ݿ����ݣ�
    this->readDatabaseData(incident);
    if (!incident.succeeded()) return;

    // �����������ż�¼������
    NativeSmRecordFilter smsRecordFilter;

    // �������������¼���������
    SeekEventHandler *smSeekEventHandlers[threadNum];
    for (uint32_t i = 0; i < threadNum; i++) {
        smSeekEventHandlers[i] = new NativeSmSeekEventHandler();
    }

    // ������������
    SqliteDatabaseSeeker smsSqliteDatabaseSeeker(this->database, this->smsTable, &smsRecordFilter,
                                                 false);
    smsSqliteDatabaseSeeker.seek(threadNum, this->seekProgressIndicator, smSeekEventHandlers,
                                 incident);
    if (!incident.succeeded()) return;

    // �ϲ����������
    for (uint32_t i = 0; i < threadNum; i++) {
        NativeSmSeekEventHandler *smSeekEventHandler = (NativeSmSeekEventHandler *) smSeekEventHandlers[i];
        vector<shared_ptr<SmShortMessage>> _smShortMessages = smSeekEventHandler->getSmShortMessages();
        this->smShortMessages.insert(this->smShortMessages.end(), _smShortMessages.begin(),
                                     _smShortMessages.end());
        delete smSeekEventHandler;
    }

    return;
}

vector<shared_ptr<SmShortMessage>> SmShortMessageSeeker::getShortMessages() const noexcept {
    return this->smShortMessages;
}

void SmShortMessageSeeker::reset() noexcept {
    this->database.close();
    this->smShortMessages.clear();
}

