//
// Created by Prasad on 02/03/2025.
//
#include <jni.h>
#include <android/log.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/time.h>

#define LOG_TAG "NativeEarlyExec"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Function to get startup timestamp
long long getStartupTime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000LL) + (time.tv_usec / 1000); // Convert to milliseconds
}

// Early execution function - runs before Application class
__attribute__((constructor)) void early_exec() {
    long long startupTime = getStartupTime();
    LOGE("Native Library : Executing before Application class!");

    // Log startup time
    LOGE("App Startup Time: %lld ms", startupTime);
}

// JNI function to expose startup time to app
extern "C"
JNIEXPORT jlong JNICALL
Java_com_yinkozi_earlyexecution_modules_NativeCommunicationModule_getStartupTime(JNIEnv *env,
                                                                                 jobject thiz) {
    return getStartupTime();
}