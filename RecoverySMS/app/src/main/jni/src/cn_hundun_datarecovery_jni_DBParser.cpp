#include "../include/cn_hundun_datarecovery_jni_DBParser.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <android/log.h>


using namespace std;

#include "../include/CommonFacility.h"
#include "../include/Incident.h"

using namespace hudun::common;

#include "../include/SmShortMessageSeeker.h"

using namespace hudun::sqlite::sm;

#define TAG "scanning"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

JNIEXPORT jobject JNICALL
Java_cn_hundun_datarecovery_jni_DBParser_getSms(JNIEnv *env, jobject instance, jstring fileName_) {
    const char *fileName = env->GetStringUTFChars(fileName_, 0);

    jclass list_cls = env->FindClass("java/util/ArrayList");
    //获得得构造函数Id
    jmethodID list_costruct = env->GetMethodID(list_cls, "<init>", "()V");
    //创建一个Arraylist集合对象
    jobject list_obj = env->NewObject(list_cls, list_costruct);

    jmethodID ArrayList_add = env->GetMethodID(list_cls, "add", "(Ljava/lang/Object;)Z");

    jclass sms_cls = env->FindClass("cn/hundun/datarecovery/jni/SMSEntity");
    jmethodID construct_sms = env->GetMethodID(sms_cls, "<init>", "()V");
    jfieldID thread_id = env->GetFieldID(sms_cls, "thread_id", "J");
    jfieldID _address = env->GetFieldID(sms_cls, "address", "Ljava/lang/String;");
    jfieldID _person = env->GetFieldID(sms_cls, "person", "J");
    jfieldID _date = env->GetFieldID(sms_cls, "date", "J");
    jfieldID _protocol = env->GetFieldID(sms_cls, "protocol", "J");
    jfieldID _type = env->GetFieldID(sms_cls, "type", "J");
    jfieldID _status = env->GetFieldID(sms_cls, "status", "J");
    jfieldID _subject = env->GetFieldID(sms_cls, "subject", "Ljava/lang/String;");
    jfieldID _body = env->GetFieldID(sms_cls, "body", "Ljava/lang/String;");
    jfieldID _service_center = env->GetFieldID(sms_cls, "service_center", "Ljava/lang/String;");
    LOGI("JNI file name = %s", fileName);

// TODO
    uint32_t threadNum = 0;
// 由系统决定搜索线程数量；
    threadNum = thread::hardware_concurrency() - 1;
    if (threadNum <= 0)
        threadNum = 1;

    LOGI("thread number ====== %d", threadNum);

// 创建短信搜索器；
    SmShortMessageSeeker smShortMessageSeeker(fileName);
// 搜索；
    Incident incident;
    smShortMessageSeeker.seek(threadNum, incident);
    if (!incident.succeeded()) {
        LOGI("  incident succeeded ==== \n%s", incident.toString().c_str());
        cout << "ERROR: " << incident.toString() << endl;
        return NULL;
    }
    LOGI("================================ start ==================================");

    vector<shared_ptr<SmShortMessage>> smShortMessages = smShortMessageSeeker.getShortMessages();
    long size = smShortMessages.size();
    LOGI("smShorMessages size == %ld", size);
    for (shared_ptr<SmShortMessage> smShortMessage:smShortMessages) {
        jobject obj_sms = env->NewObject(sms_cls, construct_sms);
        int threadId = smShortMessage->getThreadId();
        LOGI("threadId %d", threadId);
        env->SetLongField(obj_sms, thread_id, threadId);

        string address = smShortMessage->getAddress();
        const char *address_ = address.c_str();
        LOGI("address %s", address_);
        jstring a = env->NewStringUTF(address_);
        env->SetObjectField(obj_sms, _address, a);
        env->DeleteLocalRef(a);

        int person = smShortMessage->getPerson();
        LOGI("person %d", person);
        env->SetLongField(obj_sms, _person, person);

        int64_t date = smShortMessage->getDate();
        LOGI("date %lld", date);
        env->SetLongField(obj_sms, _date, date);

        int protocol = smShortMessage->getProtocol();
        LOGI("protocol %d", protocol);
        env->SetLongField(obj_sms, _protocol, protocol);

        int type = smShortMessage->getType();
        LOGI("type %d", type);
        env->SetLongField(obj_sms, _type, type);

        int status = smShortMessage->getStatus();
        LOGI("status %d", status);
        env->SetLongField(obj_sms, _status, status);

        string subject = smShortMessage->getSubject();
        LOGI("subject %s", subject.c_str());
        jstring s = env->NewStringUTF(subject.c_str());
        env->SetObjectField(obj_sms, _subject, s);
        env->DeleteLocalRef(s);

        string body = smShortMessage->getBody();
        LOGI("body %s", body.c_str());
        jstring b = env->NewStringUTF(body.c_str());
        env->SetObjectField(obj_sms, _body, b);
        env->DeleteLocalRef(b);

        string serviceCenter = smShortMessage->getServiceCenter();
        LOGI("serviceCenter %s", serviceCenter.c_str());
        jstring ser = env->NewStringUTF(serviceCenter.c_str());
        env->SetObjectField(obj_sms, _service_center, ser);
        env->DeleteLocalRef(ser);
        env->CallBooleanMethod(list_obj, ArrayList_add, obj_sms);
//        env->DeleteLocalRef(obj_sms);
    }

//    smsStream.close();

    LOGI("============ sms scanning end ============");

    env->ReleaseStringUTFChars(fileName_, fileName);

    return list_obj;
}

#include "../include/ContactsCall.h"
#include "../include/ContactsCallSeeker.h"

using namespace hudun::sqlite::contacts;

JNIEXPORT jobject JNICALL
Java_cn_hundun_datarecovery_jni_DBParser_getCalls(JNIEnv
                                                  *env,
                                                  jobject instance, jstring
                                                  fileName_
) {
    const char *fileName = env->GetStringUTFChars(fileName_, 0);
    jclass list_cls = env->FindClass("java/util/ArrayList");
    //获得得构造函数Id
    jmethodID list_costruct = env->GetMethodID(list_cls, "<init>", "()V");
    //创建一个Arraylist集合对象
    jobject list_obj = env->NewObject(list_cls, list_costruct);

    jmethodID ArrayList_add = env->GetMethodID(list_cls, "add", "(Ljava/lang/Object;)Z");

    jclass cls_call = env->FindClass("cn/hundun/datarecovery/jni/CallEntity");
    jmethodID construct_sms = env->GetMethodID(cls_call, "<init>", "()V");
    jfieldID _date = env->GetFieldID(cls_call, "date", "J");
    jfieldID _type = env->GetFieldID(cls_call, "type", "J");
    jfieldID _duration = env->GetFieldID(cls_call, "duration", "J");
    jfieldID _number = env->GetFieldID(cls_call, "number", "Ljava/lang/String;");
    jfieldID _name = env->GetFieldID(cls_call, "name", "Ljava/lang/String;");

    // 创建通话记录搜索器；
    ContactsCallSeeker contactsCallSeeker(fileName);

    // 搜索；
    try {
        uint32_t threadNum = 0;
        // 由系统决定搜索线程数量；
        threadNum = thread::hardware_concurrency() - 1;
        if (threadNum <= 0)
            threadNum = 1;

        LOGI("thread number ====== %d", threadNum);
        Incident incident;
        contactsCallSeeker.seek(threadNum, incident);
        if (!incident.succeeded()) {
            LOGI("incident is not succeeded");
            return list_obj;
        }
    }
    catch (CommonException &e) {
        LOGI("call exception message = %s;  functionName = %s;  fileName = %s",
             e.getMessage().c_str(), e.getFunctionName().c_str(), e.getFilename().c_str());
        return list_obj;
    }
    LOGI("================================== start ===================================");
    vector<shared_ptr<ContactsCall>> contactsCalls = contactsCallSeeker.getContactsCalls();
    long size = contactsCalls.size();
    LOGI("contactsCall size = %ld", size);
    for (shared_ptr<ContactsCall> contactsCall: contactsCalls) {
        jobject obj_sms = env->NewObject(cls_call, construct_sms);
        int64_t date = contactsCall->getDate();
        LOGI("date = %lld", date);
        env->SetLongField(obj_sms, _date, date);

        int16_t type = contactsCall->getType();
        LOGI("type = %lld", type);
        env->SetLongField(obj_sms, _type, type);

        int32_t duration = contactsCall->getDuration();
        LOGI("duration = %lld", duration);
        env->SetLongField(obj_sms, _duration, duration);

        const char *number = contactsCall->getNumber().c_str();
        LOGI("number = %s", number);
        jstring number_ = env->NewStringUTF(number);
        env->SetObjectField(obj_sms, _number, number_);
        env->DeleteLocalRef(number_);

        const char *name = contactsCall->getName().c_str();
        LOGI("name = %s", name);
        jstring name_ = env->NewStringUTF(name);
        env->SetObjectField(obj_sms, _name, name_);
        env->DeleteLocalRef(name_);

        env->CallBooleanMethod(list_obj, ArrayList_add, obj_sms);
        env->DeleteLocalRef(obj_sms);

    }

// TODO
    LOGI("================================== end ===================================");
    env->ReleaseStringUTFChars(fileName_, fileName);
    return list_obj;
}
