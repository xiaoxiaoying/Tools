package cn.hundun.datarecovery.jni;

import java.io.Serializable;

/**
 * Auth: Joe
 * Date: 2015-12-09
 * Time: 10:25
 * FIXME
 */
/*id                         primary key     integer                  与words表内的source_id关联
        thread_id              会话id，一个联系人的会话一个id，与threads表内的_id关联      integer
        address                 对方号码          text
        person                  联系人id           integer
        date                      发件日期           integer
        protocol               通信协议，判断是短信还是彩信     integer       0：SMS_RPOTO, 1：MMS_PROTO
        read                      是否阅读           integer       default 0             0：未读， 1：已读
        status                   状态                  integer       default-1           -1：接收，
        0：complete,
        64： pending,
        128： failed
        type                     短信类型
        integer
        1：inbox
        2：sent
        3：draft56
        4：outbox
        5：failed
        6：queued
        body                      内容
        service_center      服务中心号码
        subject                  主题
        */

public class SMSEntity implements Serializable {
    private long thread_id;
    private long person;
    private long date;
    private long protocol;
    private long status;
    private long type;
    private String address;
    private String body;
    private String service_center;
    private String subject;

    public void setThread_id(long thread_id) {
        this.thread_id = thread_id;
    }

    public void setPerson(long person) {
        this.person = person;
    }

    public void setDate(long date) {
        this.date = date;
    }

    public void setProtocol(long protocol) {
        this.protocol = protocol;
    }

    public void setStatus(long status) {
        this.status = status;
    }

    public void setType(long type) {
        this.type = type;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public void setBody(String body) {
        this.body = body;
    }

    public void setService_center(String service_center) {
        this.service_center = service_center;
    }

    public void setSubject(String subject) {
        this.subject = subject;
    }

    public long getThread_id() {
        return thread_id;
    }

    public long getPerson() {
        return person;
    }

    public long getDate() {
        return date;
    }

    public long getProtocol() {
        return protocol;
    }

    public long getStatus() {
        return status;
    }

    public long getType() {
        return type;
    }

    public String getAddress() {
        return address;
    }

    public String getBody() {
        return body;
    }

    public String getService_center() {
        return service_center;
    }

    public String getSubject() {
        return subject;
    }

    @Override
    public String toString() {
        return "SMSEntity{" +
                "thread_id=" + thread_id +
                ", person=" + person +
                ", date=" + date +
                ", protocol=" + protocol +
                ", status=" + status +
                ", type=" + type +
                ", address='" + address + '\'' +
                ", body='" + body + '\'' +
                ", service_center='" + service_center + '\'' +
                ", subject='" + subject + '\'' +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        SMSEntity entity = (SMSEntity) o;
        if (date == entity.getDate()&&body.equals(entity.getBody())&&address.equals(entity.getAddress())) {
            return true;
        }
        return false;
    }

    @Override
    public int hashCode() {
        String str = date + body + address;
        return str.hashCode();
    }
}
