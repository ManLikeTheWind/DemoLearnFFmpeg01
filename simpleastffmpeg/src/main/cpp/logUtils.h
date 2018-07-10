//
// Created by Administrator on 2018/3/1.
//

#ifndef JNIDEMO09_LOGUTILS_H
#define JNIDEMO09_LOGUTILS_H

#include <android/log.h>

#ifdef LOG_DEBUG

#define LOGI(...)
#define LOGE(...)
#define LOGE_CHARP(...)
#define LOGV(...)
#define LOGD(...)
#else
// 不能单纯的写一个参数，要写一个format,在对应的写参数
#define LOGI(...)   __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
// 不能单纯的写一个参数，要写一个format,在对应的写参数
#define LOGE(...)   __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
// 不能单纯的写一个参数，要写一个format,在对应的写参数
#define LOGE_CHARP(...)   __android_log_write(ANDROID_LOG_ERROR,LOG_TAG,## __VA_ARGS__)
// 不能单纯的写一个参数，要写一个format,在对应的写参数
//#define LOGE_CHARP  __android_log_write(ANDROID_LOG_ERROR,LOG_TAG,##__VA_ARGS__)
// 不能单纯的写一个参数，要写一个format,在对应的写参数
#define LOGV(...)   __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
// 不能单纯的写一个参数，要写一个format,在对应的写参数
#define LOGD(...)   __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#endif

#ifndef LOG
#define LOG(priority,tag,...)\
    LOG_PRI(ANDROID_##priority,tag,_VA_ARGS_)//是不是少一个[ _ ],应该是 __VA_ARGS__
#endif

#ifndef LOG_PRI
#define LOG_PRI(priority,tag,...)\
    __android_log_print(priority,tag,__VA_ARGS__)
#endif


#ifndef LOG_ASSERT
#define LOG_ASSERT(cond, ...) LOG_FATAL_IF(!(cond), ## __VA_ARGS__)
#endif

#ifndef LOG_FATAL_IF
#define LOG_FATAL_IF(cond, ...) LOG_ALWAYS_FATAL_IF(cond, ## __VA_ARGS__)
#endif

#ifndef LOG_ALWAYS_FATAL_IF
#define LOG_ALWAYS_FATAL_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)android_printAssert(#cond, LOG_TAG, ## __VA_ARGS__)) \
    : (void)0 )
#endif

#ifndef CONDITION
#define CONDITION(cond)     (__builtin_expect((cond)!=0, 0))
#endif

#define android_printAssert(a, b, ...) printf("%s: ", __VA_ARGS__)

#endif //JNIDEMO09_LOGUTILS_H
