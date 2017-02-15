#define _LARGEFILE64_SOURCE

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

    LOGI("===================== start scanning ======================");
    // we read 512 bytes at a time and only check the first 4 bytes of the buffer for jpeg signatures
    while (fread(buffer, blockSize, 1, inptr) == 1) {
        /* when already writing data to build a jpeg image file keep writing to that file until we find a new image
        note the assumption that the jpegs are saved contiguously in memory such that the beginning of the jpeg
        is an indication of the previous image's completion.*/
        bool foundJpeg = false;
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
                    end = index * blockSize + i * persize;
                    long long poor = end - start;
                    BYTE *newBytes = malloc(sizeof(BYTE) * poor);
                    long long jpg_start = start - length;
                    memcpy(newBytes, buffer + jpg_start, poor);

                    long long jpg_index = search(newBytes, poor);
                    LOGI("index ==== %lld", jpg_index);
                    if (jpg_index > 0) {
                        long long jpg_end = start + jpg_index;
                        (*env)->CallVoidMethod(env, instance, interval, start, jpg_end);
                    }
                    free(newBytes);
                    start = index * blockSize + i * persize;
                }
            }
            else if (findNewImage() == true) // keep reading the card until we find the first image
            {
                foundJpeg = true;
                start = index * blockSize + i * persize;
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

    return (memcmp(bufcpy, "\xFF\xD8\xFF\xE0", 4) == 0 ||
            memcmp(bufcpy, "\xFF\xD8\xFF\xE1", 4) == 0) ? true : false;
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
    buf = malloc(sizeof(BYTE) * poor);
    if (buf == NULL) {
        LOGI("== %s 打开失败！", filePath);
        fclose(file);
        return result;
    }
    fseek_64(file, start, SEEK_SET);
    fread(buf, sizeof(BYTE) * poor, 1, file);
    result = (*env)->NewByteArray(env, sizeof(BYTE) * poor);
    (*env)->SetByteArrayRegion(env, result, 0, sizeof(BYTE) * poor, (jbyte *) buf);
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
    bool isData = false;
    for (i; i < length - 1; ++i) {

        if (byte[i] == 0xFF && byte[i + 1] == 0xDA) {
            isData = true;
        }

        if (isData && byte[i] == 0xFF && byte[i + 1] == 0xD9) {
            return i + 2;
        }
    }
    return -1;
}