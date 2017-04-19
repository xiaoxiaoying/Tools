package com.example.administrator.nrecoverphoto.util;

/**
 * Auth: Joe
 * Date: 2015-12-08
 * Time: 14:21
 * FIXME
 */
public class Constants {
    public static final String CONTACTS_DB_PATH = "/data/data/com.android.providers.contacts/databases/contacts2.db";
    public static final String CONTACTS_HEAD_PATH = "/data/data/com.android.providers.contacts/databases";
    public static final String SMS_DB_PATH = "/data/data/com.android.providers.telephony/databases/mmssms.db";
    public static final String SUPPORT_QQ_NUMBER = "mqqwpa://im/chat?chat_type=wpa&uin=71072062";
    public static final String ROOT_CMD = "chmod 777 " + SMS_DB_PATH + ";chmod 777 " + CONTACTS_DB_PATH;
    /**
     * data分区文件权限
     */
    public static final String CHMOD_USER_DATA = "chmod 777 %s \n";
    public static final String CAT_BLOCKSIZE = "df %s \n";
    public static final String CHMOD_RM_FILE = "rm -rf %s \n";

    public static final String MOUNT_CMD = "mount -o rw,remount -t %s %s /data \n";

    public static final String DD_CMD = "dd if=%s of=%s \n";
    public static final String CP_CMD = "cp %s %s \n";

    /**
     * 查看详情
     */
    public static final String LS_L = "ls -l %s \n";

}
