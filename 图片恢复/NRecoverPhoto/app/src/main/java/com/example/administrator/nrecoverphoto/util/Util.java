package com.example.administrator.nrecoverphoto.util;

import android.app.Activity;
import android.util.DisplayMetrics;

/**
 * Created by Administrator on 2016/10/12/012.
 */

public class Util {

    public static final long G = 1024 * 1024 * 1024;
    public static final long MB = 1024 * 1024;
    public static final long KB = 1024;

    /**
     * 根据手机的分辨率从 dp 的单位 转成为 px(像素)
     */
    public static int dp2px(float dpValue) {
        return (int) (dpValue * density + 0.5f);
    }

    /**
     * 文件大小格式化
     *
     * @param length 文件长度
     * @return String
     */
    public static synchronized String sizeFormat(long length) {
        String str = "";
        if (length >= G) {
            double p = (double) length / G;
            str = String.format("%.2f", p) + " G";
        } else if (length >= MB) {
            double p = (double) length / MB;
            str = String.format("%.2f", p) + " MB";
        } else if (length >= KB) {
            double p = (double) length / KB;
            str = String.format("%.2f", p) + " KB";
        } else {

            str = length + " b";
        }

        return str;
    }

    /**
     * bytes转换成十六进制字符串
     *
     * @param b byte数组
     * @return String 每个Byte值之间空格分隔
     */
    public static String byte2HexStr(byte[] b) {
        String stmp = "";
        StringBuilder sb = new StringBuilder("");
        for (int n = 0; n < b.length; n++) {
            stmp = Integer.toHexString(b[n] & 0xFF);
            sb.append((stmp.length() == 1) ? "0" + stmp : stmp);
            sb.append(" ");
        }
        return sb.toString().toUpperCase().trim();
    }

    public static int screen_width;
    public static int screen_height;
    public static float density;

    public static void initSize(Activity activity) {
        DisplayMetrics metrics = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
        screen_width = metrics.widthPixels;
        screen_height = metrics.heightPixels;
        density = activity.getResources().getDisplayMetrics().density;
    }


}
