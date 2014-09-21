LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ptrace_attach

LOCAL_SRC_FILES := ptrace_attach.c

include $(BUILD_EXECUTABLE)
