package com.xiaoxiaoying.exif.jni;

import android.support.annotation.NonNull;

import java.io.Serializable;

/**
 * Created by Administrator on 2017/4/27/027.
 * @author xiaoxiaoying
 */

public class ExifJni implements Serializable {
    static {
        System.loadLibrary("exif_jni");
    }

    public native void saveThumbnail(@NonNull String inFilePath,@NonNull String outFilePath);
    public native int queryTagExists(@NonNull String inFilePath);
    public native int pretty(@NonNull String inFilePath);

}
