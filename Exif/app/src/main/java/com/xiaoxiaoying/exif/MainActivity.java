package com.xiaoxiaoying.exif;

import android.Manifest;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ImageView;


import com.xiaoxiaoying.exif.jni.ExifJni;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.List;

import pub.devrel.easypermissions.AfterPermissionGranted;
import pub.devrel.easypermissions.EasyPermissions;

public class MainActivity extends AppCompatActivity implements EasyPermissions.PermissionCallbacks {
    private static final String JPG = "/storage/sdcard0/DCIM/Camera/IMG_20170428_151708.jpg";
    private static final int READ_OR_WRITE = 203;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        readOrWrite();
    }


    @AfterPermissionGranted(READ_OR_WRITE)
    public void readOrWrite() {
        if (EasyPermissions.hasPermissions(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
            init();
        } else {
            EasyPermissions.requestPermissions(this, getString(R.string.permission_write), READ_OR_WRITE, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        }
    }

    void init() {
        ImageView img = (ImageView) findViewById(R.id.img);
        try {
            ExifJni exif = new ExifJni();
//            int pretty = exif.openJPG(JPG);
            int pretty = exif.pretty(JPG);
            System.out.println("pretty === " + pretty);
            Bitmap bitmap = BitmapFactory.decodeStream(new FileInputStream(JPG));
            img.setImageBitmap(bitmap);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this);
    }

    @Override
    public void onPermissionsGranted(int requestCode, List<String> perms) {
        init();
    }

    @Override
    public void onPermissionsDenied(int requestCode, List<String> perms) {

    }
}
