#include <jni.h>
#include <stdio.h>
#include "exif.h"
#include <android/log.h>

#define LOG_TAG "EXIF"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define JNI_FN(A) Java_com_xiaoxiaoying_exif_jni_ ## A

JNIEXPORT void JNICALL
JNI_FN(ExifJni_saveThumbnail)(JNIEnv *env, jobject instance,
                              jstring inFilePath_, jstring outFilePath_) {
    const char *inFilePath = (*env)->GetStringUTFChars(env, inFilePath_, 0);
    const char *outFilePath = (*env)->GetStringUTFChars(env, outFilePath_, 0);

    // TODO

    (*env)->ReleaseStringUTFChars(env, inFilePath_, inFilePath);
    (*env)->ReleaseStringUTFChars(env, outFilePath_, outFilePath);
}

JNIEXPORT jint JNICALL
JNI_FN(ExifJni_queryTagExists)(JNIEnv *env, jobject instance, jstring inFilePath_) {
    const char *inFilePath = (*env)->GetStringUTFChars(env, inFilePath_, 0);
    int sts, result;
    void **ifdTableArray = createIfdTableArray(inFilePath, &result);
    if (!ifdTableArray) {
        LOGI("createIfdTableArray: ret=%d\n", result);
        return 0;
    }

    sts = queryTagNodeIsExist(ifdTableArray, IFD_GPS, TAG_GPSLatitude);
    LOGI("GPSLatitude tag is %s in [%s]\n", (sts) ? "exists" : "not exists", inFilePath);
    freeIfdTableArray(ifdTableArray);
    (*env)->ReleaseStringUTFChars(env, inFilePath_, inFilePath);
    return sts;
}

JNIEXPORT jint JNICALL
JNI_FN(ExifJni_pretty)(JNIEnv *env, jobject instance, jstring inFilePath_) {
    const char *inFilePath = (*env)->GetStringUTFChars(env, inFilePath_, 0);

    void **ifdArray;
    TagNodeInfo *tag;
    int i, result;
    // parse the JPEG header and create the pointer array of the IFD tables
    ifdArray = createIfdTableArray(inFilePath, &result);
    // check result status
    switch (result) {
        case 0: // no IFDs
            LOGI("[%s] does not seem to contain the Exif segment.\n", inFilePath);
            break;
        case ERR_READ_FILE:
            LOGI("failed to open or read [%s].\n", inFilePath);
            break;
        case ERR_INVALID_JPEG:
            LOGI("[%s] is not a valid JPEG file.\n", inFilePath);
            break;
        case ERR_INVALID_APP1HEADER:
            LOGI("[%s] does not have valid Exif segment header.\n", inFilePath);
            break;
        case ERR_INVALID_IFD:
            LOGI("[%s] contains one or more IFD errors. use -v for details.\n", inFilePath);
            break;
        default:
            LOGI("[%s] createIfdTableArray: result=%d\n", inFilePath, result);
            break;
    }

    if (!ifdArray) {
        return 0;
    }
    // or dumpIfdTableArray(ifdArray);
    dumpIfdTableArray(ifdArray);
    // get [Model] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_Model);
    if (tag) {
        if (!tag->error) {
            LOGI("0th IFD : Model = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [Make] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_Make);
    if (tag) {
        if (!tag->error) {
            LOGI("0th IFD : Make = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [DateTime] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_DateTime);
    if (tag) {
        if (!tag->error) {
            LOGI("0th IFD : DateTime = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [Description] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_ImageDescription);
    if (tag) {
        if (!tag->error) {
            LOGI("0th IFD : Description = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [Software] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_Software);
    if (tag) {
        if (!tag->error) {
            LOGI("0th IFD : Software = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [Artist] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_Artist);
    if (tag) {
        if (!tag->error) {
            LOGI("0th IFD : Artist = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [ImageWidth] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_ImageWidth);
    if (tag) {
        if (!tag->error) {
            LOGI("0th IFD : ImageWidth = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [ImageHeight] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_ImageHeight);
    if (tag) {
        if (!tag->error) {
            LOGI("0th IFD : ImageHeight = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [XResolution] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_XResolution);
    if (tag) {
        if (!tag->error) {
            if (tag->type == TYPE_RATIONAL)
                LOGI("0th IFD : XResolution = [%d]\n", tag->numData[0]);
        }
        freeTagInfo(tag);
    }
    // get [ResolutionUnit] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_ResolutionUnit);
    if (tag) {
        if (!tag->error) {
            if (tag->type == TYPE_SHORT)
                LOGI("0th IFD : XResolution = [%d]\n", tag->numData[0]);
        }
        freeTagInfo(tag);
    }
    // get [YResolution] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_0TH, TAG_YResolution);
    if (tag) {
        if (!tag->error) {
            if (tag->type == TYPE_RATIONAL)
                LOGI("0th IFD :YResolution = [%d]\n", tag->numData[0]);
        }
        freeTagInfo(tag);
    }
    // get [PixelXDimension] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_PixelXDimension);
    if (tag) {
        if (!tag->error) {
            if (tag->type == TYPE_LONG)
                LOGI("0th IFD :PixelXDimension = [%d]\n", tag->numData[0]);
        }
        freeTagInfo(tag);
    }
    // get [PixelYDimension] tag value from 0th IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_PixelYDimension);
    if (tag) {
        if (!tag->error) {
            if (tag->type == TYPE_LONG)
                LOGI("0th IFD :PixelYDimension = [%d]\n", tag->numData[0]);
        }
        freeTagInfo(tag);
    }


    // get [MakerNote] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_MakerNote);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : MakerNote = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [UserComment] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_UserComment);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : UserComment = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [DateTimeOriginal] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_DateTimeOriginal);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : DateTimeOriginal = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [DateTimeDigitized] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_DateTimeDigitized);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : DateTimeDigitized = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [SubSecTime] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_SubSecTime);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : SubSecTime = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [SubSecTimeOriginal] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_SubSecTimeOriginal);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : SubSecTimeOriginal = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [SubSecTimeDigitized] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_SubSecTimeDigitized);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : SubSecTimeDigitized = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [ImageUniqueID] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_ImageUniqueID);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : ImageUniqueID = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
// get [CameraOwnerName] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_CameraOwnerName);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : CameraOwnerName = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
// get [BodySerialNumber] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_BodySerialNumber);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : BodySerialNumber = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
// get [LensMake] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_LensMake);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : LensMake = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // get [LensMake] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_LensModel);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : LensMake = [%s]\n", tag->byteData);
        }
        freeTagInfo(tag);
    }
// get [LensSerialNumber] tag value from Exif IFD
    tag = getTagInfo(ifdArray, IFD_EXIF, TAG_LensSerialNumber);
    if (tag) {
        if (!tag->error) {
            LOGI("Exif IFD : LensSerialNumber = [%d]\n", tag->numData[0]);
        }
        freeTagInfo(tag);
    }

    // get [GPSLatitude] tag value from GPS IFD
    tag = getTagInfo(ifdArray, IFD_GPS, TAG_GPSLatitude);
    if (tag) {
        if (!tag->error) {
            LOGI("GPS IFD : GPSLatitude = ");
            for (i = 0; i < (int) tag->count * 2; i += 2) {
                LOGI("%u/%u ", tag->numData[i], tag->numData[i + 1]);
            }
            LOGI("\n");
        }
        freeTagInfo(tag);
    }

    tag = getTagInfo(ifdArray, IFD_GPS, TAG_GPSVersionID);
    if (tag) {
        if (!tag->error) {
            LOGI("GPS version ID: %s", tag->byteData);
        }
        freeTagInfo(tag);
    }
    // free IFD table array
    freeIfdTableArray(ifdArray);
    (*env)->ReleaseStringUTFChars(env, inFilePath_, inFilePath);
    return result;
}


