package cn.hundun.datarecovery.jni;

import java.io.Serializable;

/**
 * Auth: Joe
 * Date: 2016-01-19
 * Time: 13:54
 * FIXME
 */
public class CallEntity implements Serializable{
    private String number;
    private String name;
    private long date;
    private long duration;
    private long type;

    public String getNumber() {
        return number;
    }

    public String getName() {
        return name;
    }

    public long getDate() {
        return date;
    }

    public long getDuration() {
        return duration;
    }

    public long getType() {
        return type;
    }

    public void setNumber(String number) {
        this.number = number;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setDate(long date) {
        this.date = date;
    }

    public void setDuration(long duration) {
        this.duration = duration;
    }

    public void setType(long type) {
        this.type = type;
    }

    @Override
    public boolean equals(Object another) {
        CallEntity ano= (CallEntity) another;
        return this.date==ano.getDate();
    }

    @Override
    public int hashCode() {
        return String.valueOf(this.date).hashCode();
    }

    @Override
    public String toString() {
        return "CallEntity{" +
                "number='" + number + '\'' +
                ", name='" + name + '\'' +
                ", date=" + date +
                ", duration=" + duration +
                ", type=" + type +
                '}';
    }
}
