package com.xiaoxiaoying.test2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.widget.TextView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.ArrayList;

import cn.hundun.datarecovery.jni.CallEntity;
import cn.hundun.datarecovery.jni.DBParser;
import cn.hundun.datarecovery.jni.SMSEntity;

public class MainActivity extends AppCompatActivity {
        private static final String PATH = "/data/data/com.android.providers.contacts/databases/contacts2.db";
//    private static final String PATH = "/data/data/com.android.providers.telephony/databases/mmssms.db";
    private String destPath;
    private static final String TAG = MainActivity.class.getName();
    private ArrayList<SMSEntity> arrayList = new ArrayList<>();
    private ArrayList<CallEntity> callList = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView t = (TextView) findViewById(R.id.txt);
        try {

            DBParser dbParser = new DBParser();
            cope();
            Log.i(TAG, "dest path === " + destPath);
            if (!TextUtils.isEmpty(destPath)) {
//                arrayList = dbParser.getSms(destPath);
//                int size = arrayList.size();

                callList = dbParser.getCalls(destPath);
                int size = callList.size();
                Log.i(TAG, "size = " + size);
            }
        } catch (Exception e) {

        }

//        t.setText(dbParser.stringFromJNI());
    }

    private void cope() throws Exception {
        String[] split = PATH.split("/");
        String p = split[split.length - 1];
        destPath = getFilesDir().getAbsolutePath() + "/" + p;
        java.io.FileInputStream fosfrom = null;
        FileOutputStream fosto = null;
        try {
            try {
                File file = new File(PATH);

                fosfrom = new java.io.FileInputStream(file);

                fosto = new FileOutputStream(destPath);
                byte bt[] = new byte[2048];
                int c;

                while ((c = fosfrom.read(bt)) != -1) {
                    fosto.write(bt, 0, c); //将内容写到新文件当中
                }
            } catch (FileNotFoundException er) {
            }


        } catch (FileNotFoundException e) {
            e.printStackTrace();

        } finally {
            if (fosfrom != null)
                fosfrom.close();
            if (fosto != null)
                fosto.close();
        }
    }
}
