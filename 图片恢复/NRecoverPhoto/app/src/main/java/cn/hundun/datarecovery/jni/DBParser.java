package cn.hundun.datarecovery.jni;

import java.util.ArrayList;

/**
 * Auth: Joe
 * Date: 2015-11-27
 * Time: 14:31
 * FIXME
 */
public class DBParser {
    static {
        System.loadLibrary("recovery");
    }

    public native void getSms(String fileName, ArrayList<SMSEntity> list);
    public native void getCalls(String fileName, ArrayList<CallEntity> list);
}
