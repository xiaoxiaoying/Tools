package com.example.administrator.nrecoverphoto.bean;

import java.io.Serializable;

/**
 * Created by xiaoxaioying on 2016/9/13/013.
 */
public class ImgBean implements Serializable {

    public String img_path;
    public boolean isSelect;
    public boolean isRecover;
    public byte[] bytes;
    public long start;
    public long end;
    public boolean isErr;
    public boolean isJNI;
    public String cachePath;
    public String exCachePath;
    public boolean decodeFail;

    public ImgBean(String img_path) {
        this.img_path = img_path;
    }

    public ImgBean(String img_path, byte[] bytes) {
        this.img_path = img_path;
        this.bytes = bytes;
    }

    public ImgBean(byte[] bytes) {
        this.bytes = bytes;
    }

    public ImgBean() {

    }

    public ImgBean(long start, long end, boolean isJNI) {
        this.start = start;
        this.end = end;
        this.isJNI = isJNI;
    }

}
