
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_DISABLE_FORMAT_STRING_CHECKS := true
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
LOCAL_LDLIBS	:= -llog
LOCAL_MODULE    := photo-recover
LOCAL_SRC_FILES := recover-photo.c
$(warning $(LOCAL_SRC_FILES))

include $(BUILD_SHARED_LIBRARY)