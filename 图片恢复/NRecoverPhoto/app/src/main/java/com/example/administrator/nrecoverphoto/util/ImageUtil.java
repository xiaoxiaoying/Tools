package com.example.administrator.nrecoverphoto.util;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.widget.ImageView;

import com.example.administrator.nrecoverphoto.bean.ImgBean;
import com.example.administrator.nrecoverphoto.bean.ImgLoading;
import com.example.administrator.nrecoverphoto.listener.ImageLoadingListener;
import com.stericson.RootShell.exceptions.RootDeniedException;
import com.stericson.RootShell.execution.Command;
import com.stericson.RootTools.RootTools;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeoutException;

import cn.hundun.datarecovery.jni.PhotoRecover;


/**
 * Created by xiaoxiaoying on 2016/9/20/020.
 */
public class ImageUtil {
    private static final String TAG = ImageUtil.class.getName();
    private static volatile ImageUtil imageUtil = null;
    private Context context;
    private LruCache lruCache;
    private ExecutorService mExecutorService;
    private static final Object executorObj = new Object();
    private static final Object handObj = new Object();
    private PhotoRecover photoRecover;
    private String devPath;

    private ImageUtil(Context context) {
        this.context = context;
        photoRecover = PhotoRecover.instance();
        lruCache = new LruCache(context);
        mExecutorService = Executors.newSingleThreadExecutor();
    }

    public static ImageUtil getInstance(Context context) {
        if (imageUtil == null) {
            synchronized (ImageUtil.class) {
                if (imageUtil == null) {
                    Context mContext = context.getApplicationContext();
                    imageUtil = new ImageUtil(mContext);
                }
            }
        }
        return imageUtil;
    }

    public ImageUtil setDevPath(String devPath) {
        this.devPath = devPath;
        return imageUtil;
    }

    public void displayImage(String tag, ImageView imageView, ImgBean imgBean) {
        displayImage(tag, imageView, imgBean, 0);
    }

    public void displayImage(String tag, ImageView imageView, ImgBean imgBean, ImageLoadingListener listener) {
        displayImage(tag, imageView, imgBean, 0, 0, 0, listener);
    }

    public void displayImage(String tag, ImageView imageView, ImgBean imgBean, int placeholderId) {
        displayImage(tag, imageView, imgBean, placeholderId, 0, 0);
    }

    public void displayImage(String tag, ImageView imageView, ImgBean imgBean, int width, int height) {
        displayImage(tag, imageView, imgBean, 0, width, height);
    }

    public void displayImage(String tag, ImageView imageView, ImgBean imgBean, int placeholderId, int width, int height) {
        displayImage(tag, imageView, imgBean, placeholderId, width, height, null);
    }

    public void displayImage(String tag, ImageView imageView, ImgBean imgBean, int placeholderId, int width, int height, ImageLoadingListener loadingListener) {
        if (imgBean == null)
            return;
        Bitmap bitmap = lruCache.get(tag);
        if (bitmap == null) {
            if (placeholderId != 0) {
                imageView.setImageDrawable(getPlaceholderDrawable(placeholderId));
            }
            if (loadingListener != null)
                imageView.setTag(loadingListener);
            loadImg(imageView, imgBean, tag, width, height);
        } else {
            lruCache.clearKeyUri(tag);
            imageView.setImageBitmap(bitmap);
            if (loadingListener != null)
                loadingListener.onLoadingComplete(imageView, bitmap);
            lruCache.set(tag, bitmap);
        }


    }

    private Bitmap decodeSampledBitmapFromResource(byte[] bytes,
                                                   int reqWidth, int reqHeight, final ImgBean imgBean) {
        // 第一次解析将inJustDecodeBounds设置为true，来获取图片大小
        final BitmapFactory.Options options = new BitmapFactory.Options();
        options.inPreferredConfig = Bitmap.Config.RGB_565;
        if (reqWidth != 0 && reqHeight != 0) {
            options.inJustDecodeBounds = false;

            try {

                Bitmap bitmap = BitmapFactory.decodeByteArray(bytes, 0, bytes.length, options);
                if (bitmap != null) {
                    int width = bitmap.getWidth();
                    int height = bitmap.getHeight();
                    Matrix matrix = new Matrix();
                    float scale = (float) reqWidth / width;
                    // scale = scale < scale2 ? scale : scale2;
                    matrix.postScale(scale, scale);
                    bitmap = Bitmap.createBitmap(bitmap, 0, 0, width, height, matrix, false);
                }
                if (bitmap == null && imgBean != null) {
                    imgBean.decodeFail = true;

                }
                return bitmap;
            } catch (OutOfMemoryError error) {
                error.printStackTrace();
                return null;
            }
        }
        options.inJustDecodeBounds = true;
        if (reqWidth != 0)
            options.outWidth = reqWidth;
        if (reqHeight != 0)
            options.outHeight = reqHeight;
        BitmapFactory.decodeByteArray(bytes, 0, bytes.length, options);
        // 调用上面定义的方法计算inSampleSize值
        options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);
        // 使用获取到的inSampleSize值再次解析图片
        options.inJustDecodeBounds = false;
        try {
            return BitmapFactory.decodeByteArray(bytes, 0, bytes.length, options);
        } catch (OutOfMemoryError error) {
            error.printStackTrace();
            return null;
        }
    }

    private int calculateInSampleSize(BitmapFactory.Options options,
                                      int reqWidth, int reqHeight) {
        // 源图片的高度和宽度
        final int height = options.outHeight;
        final int width = options.outWidth;
        int inSampleSize = 1;
        if (height > reqHeight || width > reqWidth) {
            // 计算出实际宽高和目标宽高的比率
            final int heightRatio = Math.round((float) height / (float) reqHeight);
            final int widthRatio = Math.round((float) width / (float) reqWidth);
            // 选择宽和高中最小的比率作为inSampleSize的值，这样可以保证最终图片的宽和高
            // 一定都会大于等于目标的宽和高。
            inSampleSize = heightRatio < widthRatio ? heightRatio : widthRatio;
        }
        return inSampleSize;
    }

    private Drawable getPlaceholderDrawable(int placeholderResId) {
        if (placeholderResId != 0) {
            return context.getResources().getDrawable(placeholderResId);
        } else return null;
    }


    private void loadImg(final ImageView view, final ImgBean imgBean, final String tag, final int width, final int height) {
        final ImgLoading imgLoading = new ImgLoading(view, imgBean, tag, width, height);
        mExecutorService.execute(new Runnable() {
            @Override
            public void run() {
                synchronized (imgLoading) {
                    byte[] bytes = imgLoading.imgBean.bytes;
                    long start = imgLoading.imgBean.start;
                    long end = imgLoading.imgBean.end;
                    boolean isJNI = imgLoading.imgBean.isJNI;
                    String cachePath = imgBean.cachePath;
                    String exCachePath = imgBean.exCachePath;
                    String imgPath = imgLoading.imgBean.img_path;
                    if (bytes == null && !TextUtils.isEmpty(devPath)) {
                        if (isJNI)
                            bytes = photoRecover.getImgData(devPath, start, end);
                        else {

                            if (!TextUtils.isEmpty(imgPath)) {
                                File imgFile = new File(imgPath);
                                if (imgFile.exists()) {
                                    try {
                                        FileInputStream fileInputStream = new FileInputStream(imgPath);
                                        bytes = new byte[(int) imgFile.length()];
                                        fileInputStream.read(bytes);
                                        fileInputStream.close();
//                                        imgFile.delete();
                                    } catch (Exception e) {
                                        try {
                                            String p = String.format(Constants.CHMOD_USER_DATA, imgPath.substring(0, imgPath.lastIndexOf("/")) + "/*");
                                            Command command = new Command(0, p);
                                            RootTools.getShell(true).add(command).finish();
                                            FileInputStream fileInputStream = new FileInputStream(imgPath);
                                            bytes = new byte[(int) imgFile.length()];
                                            fileInputStream.read(bytes);
                                            fileInputStream.close();
                                        } catch (IOException e1) {
                                            e1.printStackTrace();
                                        } catch (TimeoutException e1) {
                                            e1.printStackTrace();
                                        } catch (RootDeniedException e1) {
                                            e1.printStackTrace();
                                        }
                                        e.printStackTrace();
                                    }
                                }

                            }

                        }
                    }
                    if (bytes == null) {
                        imgLoading.imgBean.isErr = true;
                        return;
                    }
                    imgLoading.imgBean.isErr = false;
//                    imgBean.bytes = bytes;
                    Bitmap bitmap = decodeSampledBitmapFromResource(bytes, width, height, imgBean);
                    if (bitmap != null) {
                        lruCache.set(tag, bitmap);
                    }
                    Message msg = handler.obtainMessage();
                    msg.what = 3;
                    Bundle bundle = new Bundle();
                    bundle.putString("tag", imgLoading.tag);
                    bundle.putBoolean("decodeFail", imgBean.decodeFail);
                    msg.setData(bundle);
                    msg.obj = imgLoading.view;
                    handler.sendMessage(msg);
                }
            }

        });
    }

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (msg.what == 3) {
                synchronized (handObj) {
                    ImageView view = (ImageView) msg.obj;
                    Bundle bundle = msg.getData();
                    if (bundle != null && view != null) {
                        String tag = bundle.getString("tag");
                        if (!TextUtils.isEmpty(tag)) {
                            Bitmap bitmap = lruCache.get(tag);
                            ImageLoadingListener listener = (ImageLoadingListener) view.getTag();
                            if (bitmap != null) {
                                view.setImageBitmap(bitmap);
                                if (listener != null)
                                    listener.onLoadingComplete(view, bitmap);
                            } else {
                                if (listener != null)
                                    listener.onLoadingErr();
                            }
                        }
                    }
                }
            }
        }
    };

    private boolean isViewCollected(ImageView view) {
        if (view != null) {
            if (view.isClickable()) {
                return true;
            }
        }
        return false;
    }
}
