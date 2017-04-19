package com.example.administrator.nrecoverphoto;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.example.administrator.nrecoverphoto.bean.ImgBean;
import com.example.administrator.nrecoverphoto.util.ImageUtil;
import com.example.administrator.nrecoverphoto.util.Util;

import cn.hundun.datarecovery.jni.PhotoRecover;

/**
 * Created by Administrator on 2016/10/12/012.
 */

public class MainAdapter extends ArrayAdapter<ImgBean> {
    private LayoutInflater inflater;
    private PhotoRecover photoRecover;
    private String filePath;
    private String path;
    private int with;
    private ViewGroup.LayoutParams params;

    public MainAdapter(Context context, String filePath, String path) {
        super(context);
        this.filePath = filePath;
        this.path = path;
        photoRecover = PhotoRecover.instance();
        inflater = LayoutInflater.from(context);
        int size = Util.dp2px(12);
        with = (Util.screen_width - size) / 3;
        params = new ViewGroup.LayoutParams(with, with);
    }


    @Override
    public void onBindView(RecyclerView.ViewHolder holder, int position, ImgBean object) {

        if (object != null && holder != null) {
            ViewHolder viewHolder = (ViewHolder) holder;
            ImageView img = (ImageView) viewHolder.itemView;
            String tag = String.valueOf(object.start) + String.valueOf(object.end);
            Log.i("888", "position =============== " + position + " tag = " + tag);
            ImageUtil.getInstance(getContext()).displayImage(tag, img, object, R.mipmap.ic_launcher, with, with);
        }

    }

    @Override
    public RecyclerView.ViewHolder getViewHolder(ViewGroup parent, int viewType) {

        View view = inflater.inflate(R.layout.item, parent, false);

        view.setLayoutParams(params);

        return new ViewHolder(view);
    }

    private class ViewHolder extends RecyclerView.ViewHolder {

        ViewHolder(View itemView) {
            super(itemView);
        }
    }

    interface Listener {
        void onClickListener(ImgBean photoBean);
    }
}
