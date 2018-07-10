/**
 * 最简单的基于FFmpeg的Helloworld例子-安卓
 * Simplest FFmpeg Android Helloworld
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 *
 * 本程序是移植FFmpeg到安卓平台的最简单程序。它可以打印出FFmpeg类库的下列信息：
 * Protocol:  FFmpeg类库支持的协议
 * AVFormat:  FFmpeg类库支持的封装格式
 * AVCodec:   FFmpeg类库支持的编解码器
 * AVFilter:  FFmpeg类库支持的滤镜
 * Configure: FFmpeg类库的配置信息
 *
 * This is the simplest program based on FFmpeg in Android. It can show following
 * informations about FFmpeg library:
 * Protocol:  Protocols supported by FFmpeg.
 * AVFormat:  Container format supported by FFmpeg.
 * AVCodec:   Encoder/Decoder supported by FFmpeg.
 * AVFilter:  Filters supported by FFmpeg.
 * Configure: configure information of FFmpeg.
 */

#include <jni.h>
#include <string>
#include "logUtils.h"
extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
}

#undef LOG_TAG
#define LOG_TAG "Information-Application"

//FIX
struct URlProtocol;
extern "C"{

JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_stringFromJNI(
        JNIEnv *env,
        jobject instance) {
/*    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());*/

    char info[10000] = { 0 };
    sprintf(info, "%s\n", avcodec_configuration());
    return env->NewStringUTF(info);
}

/** Protocol:  FFmpeg类库支持的协议 Information */
JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_urlprotocolinfo( JNIEnv *env, jobject instance){
    char info[10000]={0};
    av_register_all();
    struct URlProtocol *pup=NULL;
    //Input
    struct URlProtocol **p_temp=&pup;
    avio_enum_protocols((void **)p_temp,0);
    while ((*p_temp)!=NULL){
        sprintf(info,"%s[In ][%10s]\n",info,avio_enum_protocols((void**)p_temp,0));
    }
    pup=NULL;
    //Output
    avio_enum_protocols((void**)p_temp,1);
    while ((*p_temp)!=NULL){
        sprintf(info,"%s[Out][%10s]\n",info,avio_enum_protocols((void**)p_temp,1));
    }
    LOGE("Protocol %s",info);
    return env->NewStringUTF(info);
}
/** AVFormat:  FFmpeg类库支持的封装格式 */
JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_avformatinfo(JNIEnv *env, jobject instance) {
    char info[40000]={0};
    av_register_all();

    AVInputFormat *if_temp=av_iformat_next(NULL);
    AVOutputFormat *of_temp=av_oformat_next(NULL);

    //Input
    while (if_temp!=NULL){
        sprintf(info,"%s[In ][%10s]\n",info,if_temp->name);
        if_temp=if_temp->next;
    }
    //Output
    while (of_temp!=NULL){
        sprintf(info,"%s[Out][%10s]\n",info,of_temp->name);
        of_temp=of_temp->next;
    }
    LOGE("avformat  %s",info);
    return env->NewStringUTF(info);
}

/**  AVCodec:   FFmpeg类库支持的编解码器*/
JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_avcodecinfo(JNIEnv *env, jobject instance){
    char info[40000]={0};
    av_register_all();

    AVCodec *c_temp=av_codec_next(NULL);
    while (c_temp!=NULL){
        if (c_temp->decode!=NULL){
            sprintf(info,"%s[Dec]",info);
        } else{
            sprintf(info,"%s[Enc]",info);
        }

        switch (c_temp->type){
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info,"%s[Video]",info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info,"%s[Audio]",info);
                break;
            default:
                sprintf(info,"%s[Other]",info);
                break;
        }
        sprintf(info,"%s[%10s]\n",info,c_temp->name);
        c_temp=c_temp->next;
    }

    LOGE("avcodec  %s",info);

    return env->NewStringUTF(info);
}

/** AVFilter:  FFmpeg类库支持的滤镜*/
JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_avfilterinfo(JNIEnv *env, jobject instance){
    char info[40000]={0};
    av_register_all();
    AVFilter *f_temp= (AVFilter *) avfilter_next(NULL);
    while (f_temp!=NULL){
        sprintf(info,"%s[%10s]\n",info,f_temp->name);
    }
    LOGE("avfilter %s",info);
    return env->NewStringUTF(info);
}

/** Configure: FFmpeg类库的配置信息 */
JNIEXPORT jstring JNICALL
Java_com_dxtest_simpleastffmpeg_JniFFmpegHelloWorld_configurationinfo(JNIEnv *env,  jobject instance){
    char info[40000]={0};
    av_register_all();

    sprintf(info,"%s\n",avcodec_configuration());

    LOGE("avcodec-configuration %s",info);

    return env->NewStringUTF(info);
}


}


