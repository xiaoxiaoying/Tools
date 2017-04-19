package com.example.administrator.nrecoverphoto;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PersistableBundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.text.SpannableStringBuilder;
import android.text.Spanned;
import android.text.TextUtils;
import android.text.style.ForegroundColorSpan;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.example.administrator.nrecoverphoto.util.Util;

import cn.hundun.datarecovery.jni.PhotoRecover;

/**
 * Created by Administrator on 2016/11/10/010.
 */

public class ShowTextActivity extends AppCompatActivity {
    private TextView textView;
    private Intent intent;
    private long start;
    private long end;
    private EditText editText;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_txt);
        textView = (TextView) findViewById(R.id.txt);
        editText = (EditText) findViewById(R.id.edit);
        intent = getIntent();
        start = intent.getLongExtra("start", 0);
        end = intent.getLongExtra("end", 0);

        if (end == 0 || start == 0)
            finish();
        new Thread(new Runnable() {
            @Override
            public void run() {
                byte[] bytes = PhotoRecover.instance().getImgData("/dev/block/mmcblk0", start, end);
                int length = bytes.length;
                String b = Util.byte2HexStr(bytes);
                Message message = handler.obtainMessage();
                message.obj = b;
                message.arg1 = length;

                message.what = 1;
                handler.sendMessage(message);
            }
        }).start();
    }


    private static final String SOI = "FF D8";
    private static final String EOI = "FF D9";
    private static final String SOS = "FF DA";
    private static final String DQT = "FF DB";
    private static final String COM = "FF FE";
    private static final String SOF0 = "FF C0";
    private static final String APP0 = "FF E0";
    private static final String APP01 = "FF E1";
    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (msg.what == 1) {
                String t = (String) msg.obj;
                int length = msg.arg1;
                if (TextUtils.isEmpty(t))
                    return;
                t = "文件长度 " + Util.sizeFormat(length) + "\n" + t;
                SpannableStringBuilder spannable = new SpannableStringBuilder(t);
                int soi = t.indexOf(SOI);
                if (soi > 0) {
                    ForegroundColorSpan soiColor = new ForegroundColorSpan(getResources().getColor(R.color.colorAccent));
                    spannable.setSpan(soiColor, soi, soi + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                }

                int eoi = t.indexOf(EOI);
                if (eoi > 0) {
                    ForegroundColorSpan soiColor = new ForegroundColorSpan(getResources().getColor(R.color.l));
                    spannable.setSpan(soiColor, eoi, eoi + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    int lastEoi = t.lastIndexOf(EOI);
                    if (lastEoi != eoi) {
                        spannable.setSpan(soiColor, lastEoi, lastEoi + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    }
                }
                int sos = t.indexOf(SOS);
                if (sos > 0) {
                    ForegroundColorSpan soiColor = new ForegroundColorSpan(getResources().getColor(R.color.m));
                    spannable.setSpan(soiColor, sos, sos + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    int lastEoi = t.lastIndexOf(SOS);
                    if (lastEoi != sos) {
                        spannable.setSpan(soiColor, lastEoi, lastEoi + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    }
                }
                int dqt = t.indexOf(DQT);
                if (dqt > 0) {
                    ForegroundColorSpan soiColor = new ForegroundColorSpan(getResources().getColor(R.color.r));
                    spannable.setSpan(soiColor, dqt, dqt + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    int lastEoi = t.lastIndexOf(DQT);
                    if (lastEoi != dqt) {
                        spannable.setSpan(soiColor, lastEoi, lastEoi + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    }
                }
                int com = t.indexOf(COM);
                if (com > 0) {
                    ForegroundColorSpan soiColor = new ForegroundColorSpan(getResources().getColor(R.color.y));
                    spannable.setSpan(soiColor, com, com + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    int lastEoi = t.lastIndexOf(COM);
                    if (lastEoi != com) {
                        spannable.setSpan(soiColor, lastEoi, lastEoi + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    }
                }
                int sofo = t.indexOf(SOF0);
                if (sofo > 0) {
                    ForegroundColorSpan soiColor = new ForegroundColorSpan(getResources().getColor(R.color.colorPrimaryDark));
                    spannable.setSpan(soiColor, sofo, sofo + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    int lastEoi = t.lastIndexOf(SOF0);
                    if (lastEoi != sofo) {
                        spannable.setSpan(soiColor, lastEoi, lastEoi + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    }
                }
                int appo = t.indexOf(APP0);
                if (appo > 0) {
                    ForegroundColorSpan soiColor = new ForegroundColorSpan(Color.YELLOW);
                    spannable.setSpan(soiColor, appo, appo + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    int lastEoi = t.lastIndexOf(APP0);
                    if (lastEoi != appo) {
                        spannable.setSpan(soiColor, lastEoi, lastEoi + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    }
                }
                int appo1 = t.indexOf(APP01);
                if (appo1 > 0) {
                    ForegroundColorSpan soiColor = new ForegroundColorSpan(Color.GREEN);
                    spannable.setSpan(soiColor, appo1, appo1 + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    int lastEoi = t.lastIndexOf(APP01);
                    if (lastEoi != appo1) {
                        spannable.setSpan(soiColor, lastEoi, lastEoi + 5, Spanned.SPAN_EXCLUSIVE_INCLUSIVE);
                    }
                }
                textView.setText(spannable);
            }
        }
    };

    public void save(View view) {
        String path = editText.getText().toString().trim();
        if (TextUtils.isEmpty(path))
            return;
        
        PhotoRecover.instance().saveImgFile("/dev/block/mmcblk0", "/storage/sdcard0/HDImg/" + path, start, end);
    }


}
