LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ptrace_hook

LOCAL_SRC_FILES := ptrace_hook.c

include $(BUILD_EXECUTABLE)
