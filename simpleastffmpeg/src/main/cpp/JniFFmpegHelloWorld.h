/*
Administrator="ADSF"
*.fileName: JniFFmpegApplicaion.h
*..project:DemoLearnFFmpeg01
*.....user:dongxiang 
*.....date:2018/7/10
*..descrip:
//
*/
#ifndef DEMOLEARNFFMPEG01_JNIFFMPEGHELLOWORLD_H
#define DEMOLEARNFFMPEG01_JNIFFMPEGHELLOWORLD_H



#include <jni.h>


extern "C"{
JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_stringFromJNI(JNIEnv *env, jobject instance);

JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_urlprotocolinfo(JNIEnv *env, jobject instance);


JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_avformatinfo(JNIEnv *env, jobject instance) ;


JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_avcodecinfo(JNIEnv *env, jobject instance);



JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_avfilterinfo(JNIEnv *env, jobject instance);


JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_configurationinfo(JNIEnv *env,  jobject instance);

};
#endif //DEMOLEARNFFMPEG01_JNIFFMPEGAPPLICAION_H
