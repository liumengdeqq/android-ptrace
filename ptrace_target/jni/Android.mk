LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ptrace_test

LOCAL_SRC_FILES := target.c

include $(BUILD_EXECUTABLE)
