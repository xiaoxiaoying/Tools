package com.example.administrator.nrecoverphoto.bean;

import android.widget.ImageView;

/**
 * Created by xiaoxiaoying on 2016/10/24/024.
 */

public class ImgLoading {

    public final ImageView view;
    public final ImgBean imgBean;
    public final String tag;
    public final int width;
    public final int height;

    public ImgLoading(ImageView view, ImgBean imgBean, String tag, int width, int height) {
        this.view = view;
        this.imgBean = imgBean;
        this.tag = tag;
        this.width = width;
        this.height = height;
    }
}
