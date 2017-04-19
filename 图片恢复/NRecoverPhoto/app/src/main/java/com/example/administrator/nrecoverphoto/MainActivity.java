package com.example.administrator.nrecoverphoto;

import android.content.Intent;
import android.graphics.Rect;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.administrator.nrecoverphoto.bean.ImgBean;
import com.example.administrator.nrecoverphoto.listener.PhotoListener;
import com.example.administrator.nrecoverphoto.util.Util;

import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import cn.hundun.datarecovery.jni.PhotoRecover;

public class MainActivity extends AppCompatActivity implements PhotoListener, MainAdapter.Listener {
    private static Object object = new Object();
    private static final Object objectByte = new Object();
    private PhotoRecover photoRecover;
    //                private String filePath = "/storage/sdcard0/card.raw";
//    private String filePath = "/dev/block/dm-1";
    private String filePath = "/dev/block/mmcblk0";
    //    private String filePath = "/dev/block/sda16";
    private String dd = "/data/data/cn.hudun.wizard/cache/hddd";
    private File cacheDir;
    private String path;
    private RecyclerView recyclerView;
    private TextView textView;
    private long fileLength;
    private String fileSize;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        photoRecover = PhotoRecover.instance();
        cacheDir = getExternalCacheDir();
        if (cacheDir != null && cacheDir.exists()) {
            path = cacheDir.getAbsolutePath() + "/out";

        }


        initView();
        startRecoverPhoto();
    }

    private String hint;
    private MainAdapter adapter;
    private String apkRoot;

    private void initView() {
        Util.initSize(this);
        apkRoot = "chmod 777 /dev/block/mmcblk0\n";
        textView = (TextView) findViewById(R.id.txt);
        recyclerView = (RecyclerView) findViewById(R.id.recycler_view);
        recyclerView.setLayoutManager(new GridLayoutManager(this, 3));
        photoRecover.setScanningListener(this);
        fileLength = photoRecover.getFileSize(filePath);
        fileSize = Util.sizeFormat(fileLength);
        Log.i("888", "file length = " + fileSize);
        final int size = Util.dp2px(2);
        hint = getString(R.string.hint);
        recyclerView.addItemDecoration(new RecyclerView.ItemDecoration() {
            @Override
            public void getItemOffsets(Rect outRect, View view, RecyclerView parent, RecyclerView.State state) {
//                super.getItemOffsets(outRect, view, parent, state);
                outRect.set(size, size, size, size);

            }
        });
        adapter = new MainAdapter(this, filePath, path);
        adapter.setOnClickListener(this);
        recyclerView.setAdapter(adapter);

    }

    private ExecutorService executorService;

    private ExecutorService getExecutorService() {
        synchronized (object) {
            if (executorService == null)
                executorService = Executors.newCachedThreadPool();
            return executorService;
        }
    }

    private void startRecoverPhoto() {
        getExecutorService().execute(new Runnable() {
            @Override
            public void run() {
                File file = new File(path);
                if (!file.exists())
                    file.mkdirs();
                Log.e("888", "path = " + path);
                try {
                    Runtime.getRuntime().exec(apkRoot);
                    fileLength = photoRecover.getFileSize(filePath);
                    fileSize = Util.sizeFormat(fileLength);
                    Log.i("888", "file length = " + fileSize);
//                    long length = photoRecover.getFileSize(filePath);
                    // /storage/sdcard0/DCIM/Camera/IMG_20160928_171201.jpg  size = 661263
                    int code = photoRecover.recover(filePath, path + "/", 4096);
//                    photoRecover.jpgFile("/storage/sdcard0/DCIM/Camera/IMG_20160928_171201.jpg");
                } catch (Exception e) {
                    e.printStackTrace();
                }


            }
        });
    }

    @Override
    public void progress(final long progress) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                synchronized (object) {
                    String read = Util.sizeFormat(progress);
                    String remaining = Util.sizeFormat(fileLength - progress);
                    textView.setText(String.format(hint, read, remaining, fileSize));
                }

            }
        });
    }

    @Override
    public void interval(final long start, final long end) {
        System.out.println("start ===" + start + " end === " + end);
        if (start > end) {
            return;
        }
        long poor = end - start;
        if (poor < Util.KB * 10) {
            return;
        }

//        boolean isBitmap = PhotoRecover.instance().isBitmap("/dev/block/mmcblk0",start,end);
//        if (!isBitmap)
//            return;

        synchronized (objectByte) {
            try {

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        adapter.add(new ImgBean(start, end, true));
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

        }

    }

    /**
     * 获取CPU架构
     *
     * @return String
     */
    private String getCPU() {
        String[] abis = new String[]{};
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
            abis = Build.SUPPORTED_ABIS;
        } else {
            abis[0] = Build.CPU_ABI;
            abis[1] = Build.CPU_ABI2;
        }
        StringBuilder abiStr = new StringBuilder();
        for (String abi : abis) {
            abiStr.append(abi);
            abiStr.append(',');
        }
        return abiStr.toString();
    }

    private Intent intent;

    @Override
    public void onClickListener(ImgBean photoBean) {

        Log.i("888", "=========================");

        if (photoBean != null) {
            intent = new Intent(this, ShowTextActivity.class);
            intent.putExtra("start", photoBean.start);
            intent.putExtra("end", photoBean.end);
            startActivity(intent);
        }
    }
}
