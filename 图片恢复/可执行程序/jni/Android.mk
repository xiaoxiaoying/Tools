
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_DISABLE_FORMAT_STRING_CHECKS := true
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
LOCAL_MODULE    := recover
LOCAL_SRC_FILES := recover.c
$(warning $(LOCAL_SRC_FILES))
# BUILD_EXECUTABLE指明生成可执行的二进制文件
include $(BUILD_EXECUTABLE)