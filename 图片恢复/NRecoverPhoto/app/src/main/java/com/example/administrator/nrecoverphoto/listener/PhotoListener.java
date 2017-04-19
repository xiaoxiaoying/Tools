package com.example.administrator.nrecoverphoto.listener;

/**
 * Created by Administrator on 2016/10/12/012.
 */

public interface PhotoListener {

    void progress(long progress);

    void interval(long start, long end);

}
