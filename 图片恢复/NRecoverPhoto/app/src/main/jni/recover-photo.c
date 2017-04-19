#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#include <android/log.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include<fcntl.h>
#include <string.h>
#include <stdint.h>
#include "jpgcom.h"

typedef uint8_t BYTE;
#define TAG "libso"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
unsigned int blockSize = 4096; // FAT file system has block size of 512 Bytes
bool stopScann = false;

// function prototypes
bool findNewImage(void);

void writeToImage(BYTE *byte, const char *outPath);

int fseek_64(FILE *stream, int64_t offset, int origin);

int cmp(const void *a, const void *b);

int saveFile(const char *devPath, const char *outPath, long long start, long long end);

long long search(BYTE *byte, long long length);

long long parseJPG(BYTE *byte, long long length);

// 进度id
static jmethodID progress = NULL;
// 图片区间 id
static jmethodID interval = NULL;

BYTE *buffer = NULL;
BYTE *bufcpy = NULL;

JNIEXPORT jint JNICALL
Java_cn_hundun_datarecovery_jni_PhotoRecover_recover(JNIEnv *env, jobject instance,
                                                     jstring filePath_,
                                                     jstring outPath_, jint blockSize_) {
    stopScann = false;
    char lpOutPath[256] = {0};
    const char *filePath = (*env)->GetStringUTFChars(env, filePath_, 0);
    const char *outPath = (*env)->GetStringUTFChars(env, outPath_, 0);
    long long start = 0;//图片起始位置
    long long end = 0;//图片结束位置
    LOGI("block size = %d", blockSize_);
    // 判断blockSize的大小
    if (blockSize_ != 0) {
        blockSize = (unsigned int) (blockSize_ * 1000);
    }
    // 获取Java class
    jclass cls = (*env)->GetObjectClass(env, instance);
    interval = (*env)->GetMethodID(env, cls, "interval", "(JJ)V");
    progress = (*env)->GetMethodID(env, cls, "progress", "(J)V");

    //  starts off initially with no images found

    FILE *inptr = fopen(filePath, "rb");
    if (inptr == NULL) {
        LOGD("Could not open %s\n", filePath);
        return 1;
    }
    // create buffer which we will read to
    int fd = fileno(inptr);
    lseek64(fd, 0, SEEK_SET);
    buffer = malloc(blockSize);
    unsigned int persize = blockSize / 1000;
    bufcpy = malloc(persize);
    unsigned long long index = 0;
    bool foundJpeg = false;
    LOGI("===================== start scanning ======================");
    // we read 512 bytes at a time and only check the first 4 bytes of the buffer for jpeg signatures
    while (fread(buffer, blockSize, 1, inptr) == 1) {
        /* when already writing data to build a jpeg image file keep writing to that file until we find a new image
        note the assumption that the jpegs are saved contiguously in memory such that the beginning of the jpeg
        is an indication of the previous image's completion.*/
        if (stopScann)
            break;
        unsigned long long length = index * blockSize;
        (*env)->CallVoidMethod(env, instance, progress, length);
        int i = 0;
        int totalsize = 0;
        for (i; i < 1000; ++i) {
            memcpy(bufcpy, buffer + totalsize, persize);
            totalsize += persize;
            if (foundJpeg == true) {
                if (findNewImage() == true) {
                    end = length + i * persize;
                    long long poor = end - start;
                    long long jpg_start = start - length;
                    if (jpg_start > 0) {
                        BYTE *newBytes = malloc(sizeof(BYTE) * poor);
                        memcpy(newBytes, buffer + jpg_start, poor);
                        long long jpg_index = parse_jpg(newBytes, poor);
                        LOGI("index ==== %lld", jpg_index);
                        if (jpg_index > 0) {
                            long long jpg_end = start + jpg_index;
                            (*env)->CallVoidMethod(env, instance, interval, start, jpg_end);
                        }
                        free(newBytes);
                    } else
                        (*env)->CallVoidMethod(env, instance, interval, start, end);

                    start = length + i * persize;
                }
            }
            else if (findNewImage() == true) // keep reading the card until we find the first image
            {
                foundJpeg = true;
                start = length + i * persize;
            }

        }
        index++;
    }
    // free buffer since we used malloc to allocate memory for it in the heap
    free(buffer);
    fclose(inptr);
    LOGD("============== end scanning ==================");
    return 0;

}

int cmp(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

bool findNewImage(void) {

    // return (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2]== 0xff && (buffer[3]== 0xe0 || buffer[3]==0xe1) ) ? true : false;

    unsigned int sign = 0xE0FFD8FF;
    unsigned int sign1 = 0xE1FFD8FF;
    unsigned int usrc = (*(unsigned int *) bufcpy);
    return sign == usrc || sign1 == usrc;
//    return (memcmp(bufcpy, "\xFF\xD8\xFF\xE0", 4) == 0 ||
//            memcmp(bufcpy, "\xFF\xD8\xFF\xE1", 4) == 0) ? true : false;
}

void writeToImage(BYTE *bytes, const char *outPath) {
    // name found files as ###.jpg so need an extra for null character
    LOGI("outpath === %s", outPath);
    FILE *outptr = fopen(outPath, "a");
    if (outptr == NULL) {
        printf("couldn't open the outfile\n");
        return;
    }
    fwrite(&bytes[0], blockSize, 1, outptr); // write contents from buffer to the outfile
    fclose(outptr);

}

JNIEXPORT jlong JNICALL
Java_cn_hundun_datarecovery_jni_PhotoRecover_getFileSize(JNIEnv *env, jobject instance,
                                                         jstring filePath_) {
    const char *filePath = (*env)->GetStringUTFChars(env, filePath_, 0);
    // TODO
//    int su = system("su -c \"find / -perm -2000 -o -perm -4000; ps; ls\"");
//    LOGD("su %d", su);
    FILE *file = fopen(filePath, "rb");
//    int fd = open(filePath, O_RDONLY | O_LARGEFILE);
    if (file == NULL) {
        return errno;
    }
    int fd = fileno(file);
    int64_t length = lseek64(fd, 0, SEEK_END);
    fclose(file);
    return length;
}

JNIEXPORT void JNICALL
Java_cn_hundun_datarecovery_jni_PhotoRecover_stopScann(JNIEnv *env, jobject instance) {
    // TODO
    stopScann = true;
}

JNIEXPORT jbyteArray JNICALL
Java_cn_hundun_datarecovery_jni_PhotoRecover_getImgData(JNIEnv *env, jobject instance,
                                                        jstring filePath_, jlong start, jlong end) {
    BYTE *buf = NULL;
    jbyteArray result = NULL;
    const char *filePath = (*env)->GetStringUTFChars(env, filePath_, 0);
    long long poor = end - start;
    LOGI(" ===== poor %lld ======== start %lld ======== end %lld", poor, start, end);
    // TODO
    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
        return result;

    if (poor > 8 * 1024 * 1024) {
        poor = 8 * 1024 * 1024;
    }

    BYTE *buff = malloc(sizeof(BYTE) * poor);
    if (buff == NULL) {
        LOGI("== %s 打开失败！", filePath);
        fclose(file);
        return result;
    }
    fseek_64(file, start, SEEK_SET);
    fread(buff, sizeof(BYTE) * poor, 1, file);
    if (poor > 8 * 1024 * 1024) {
        long long jpg_end = parse_jpg(buff, poor);
        if (jpg_end > 0) {
            poor = jpg_end;
        }
    }
    buf = malloc(sizeof(BYTE) * poor);
    memcpy(buf, buff, poor);
    result = (*env)->NewByteArray(env, sizeof(BYTE) * poor);
    (*env)->SetByteArrayRegion(env, result, 0, sizeof(BYTE) * poor, (jbyte *) buf);
    free(buff);
    free(buf);
    fclose(file);
    return result;

}

int fseek_64(FILE *stream, int64_t offset, int origin) {
    if (feof(stream)) {
        rewind(stream);
    }
    else {
        setbuf(stream, NULL); //清空fread的缓存
    }
    int fd = fileno(stream);
    if (lseek64(fd, offset, origin) == -1) {
        return errno;
    }
    return 0;
}


JNIEXPORT jboolean JNICALL
Java_cn_hundun_datarecovery_jni_PhotoRecover_saveImgFile(JNIEnv *env, jobject instance,
                                                         jstring devPath_, jstring outPath_,
                                                         jlong start, jlong end) {
    const char *devPath = (*env)->GetStringUTFChars(env, devPath_, 0);
    const char *outPath = (*env)->GetStringUTFChars(env, outPath_, 0);

    // TODO

    return (jboolean) saveFile(devPath, outPath, start, end);
}


int saveFile(const char *devPath, const char *outPath, long long start, long long end) {
    BYTE *cacheByte;
    long long poor;
    FILE *file = fopen(devPath, "rb");
    if (file == NULL)
        return 0;

    FILE *outFile = fopen(outPath, "wb");
    if (outFile == NULL)
        return 0;

    if (start != 0 && end != 0) {
        poor = end - start;
        cacheByte = malloc(sizeof(BYTE) * poor);
        fseek_64(file, start, SEEK_SET);
        fread(cacheByte, sizeof(BYTE) * poor, 1, file);
    } else {
        int fd = fileno(file);
        int64_t length = lseek64(fd, 0, SEEK_END);
        poor = length;
        lseek64(fd, 0, SEEK_SET);
        cacheByte = malloc(sizeof(BYTE) * poor);
        fread(cacheByte, sizeof(BYTE) * poor, 1, file);
    }
    fwrite(cacheByte, sizeof(BYTE) * poor, 1, outFile);
    free(cacheByte);
    fclose(outFile);
    fclose(file);
    return 1;
}

long long search(BYTE *byte, long long length) {
    long long i = 0;
    for (i; i < length - 3; ++i) {
        unsigned int sho = (*(unsigned int *) (byte + i));

        if (sho == 0x0000D9FF) {
            return i + 2;
        }

    }
    return -1;
}

#define P_SOI 0xD8FF
#define P_APP0 0xE0
#define P_APP1 0xE1
#define P_SOF0 0xC0
#define P_SOF2 0xC2
#define P_DHT 0xC4
#define P_DQT 0xDB
#define P_DRI 0xDDFF
#define P_SOS 0xDA
#define P_COM 0xFEFF
#define P_EOI 0xD9

long long parseJPG(BYTE *byte, long long length) {
    long long i = 0;
    do {
        i += 1;
        int vue = byte[i];
        unsigned short v = 0;
        if (i < length - 2)
            v = (*(unsigned short *) (byte + (i + 2)));
        switch (vue) {
            case P_APP0:
            case P_APP1:
            case P_DQT:
            case P_SOS:
            case P_SOF0:
            case P_SOF2:
            case P_DHT:
                if (i < length - 2) {
                    i += v;
                    LOGI("========= length ==== %d", v);
                    if (v == 0)
                        return -1;
                }
                break;
            case P_EOI:
                return i + 2;
            default:
                i++;
                break;
        }

    } while (i < length && byte[i] == 0xFF);
    return -1;
}

JNIEXPORT void JNICALL
Java_cn_hundun_datarecovery_jni_PhotoRecover_jpgFile(JNIEnv *env, jobject instance, jstring file_) {
    const char *filePath = (*env)->GetStringUTFChars(env, file_, 0);

    // TODO

    LOGD("jpg file path == %s", filePath);

    FILE *file = fopen(filePath, "rb");
    if (file == NULL) {
        LOGD("err：open %s file fail", filePath);
        return;
    }
    int fd = fileno(file);
    long long len = lseek64(fd, 0, SEEK_END);
    LOGI("file length = %lld", len);
    lseek64(fd, 0, SEEK_SET);
    BYTE *byte = malloc(sizeof(BYTE) * len);
    fread(byte, len, 1, file);
    unsigned int i = (unsigned int) -1;
    LOGD("i == %d ", i);

    unsigned int ii = (i << 8) + i;
    LOGD("ii ===== %d", ii);
//    long long in = parse_jpg(byte, len);
//    LOGI("index %lld", in);
    free(byte);
    (*env)->ReleaseStringUTFChars(env, file_, filePath);
    fclose(file);
}

