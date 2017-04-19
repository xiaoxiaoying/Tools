package com.example.administrator.nrecoverphoto.listener;

import android.graphics.Bitmap;
import android.view.View;

/**
 * Created by xiaoxiaoying on 2016/10/18/018.
 */

public interface ImageLoadingListener {
    void onLoadingComplete(View view, Bitmap loadedImage);
    void onLoadingErr();
}
