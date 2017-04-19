package cn.hundun.datarecovery.jni;

import android.util.Log;

import com.example.administrator.nrecoverphoto.listener.PhotoListener;

/**
 * Created by xiaoxiaoying on 2016/10/10/010.
 */

public class PhotoRecover {
    static Object object = new Object();
    private static PhotoRecover photoRecover;
    private PhotoListener listener;

    static {
        System.loadLibrary("photo-recover");
    }

    private PhotoRecover() {

    }

    public static PhotoRecover instance() {
        synchronized (object) {
            if (photoRecover == null)
                photoRecover = new PhotoRecover();
            return photoRecover;
        }

    }

    /**
     * 扫描图片
     *
     * @param filePath  文件路径
     * @param outPath   图片保存路径
     * @param blockSize 文件块大小
     * @return int 0:扫描完毕；1：打开文件失败；2：保存文件失败
     */
    public native int recover(String filePath, String outPath, int blockSize);

    /**
     * 获取图片byte[]
     *
     * @param filePath 文件路径
     * @param start    图片起始位置
     * @param end      图片结束位置
     * @return byte[]
     */
    public native byte[] getImgData(String filePath, long start, long end);

    /**
     * 停止扫描
     */
    public native void stopScann();

    /**
     * 获取文件大小
     *
     * @param filePath 文件路径
     * @return length
     */
    public native long getFileSize(String filePath);


    public native boolean saveImgFile(String devPath, String outPath, long start, long end);

    public native boolean saveImgFile(String inPath, String outPath);

    /**
     * 图片区间
     *
     * @param start 开始下标
     * @param end   结束下标
     */
    public void interval(long start, long end) {
        if (listener != null)
            listener.interval(start, end);
    }

    /**
     * 扫描文件进度
     *
     * @param progress long / b
     */
    public void progress(long progress) {
        if (listener != null)
            listener.progress(progress);

    }


    public void setScanningListener(PhotoListener listener) {
        this.listener = listener;
    }


    public native void jpgFile(String file);


}
