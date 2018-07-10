/*
Administrator="ADSF"
*.fileName: com_dxtest_simpleastffmpeg_JniFFmpeg02Decoder.cpp
*..project:DemoLearnFFmpeg01
*.....user:dongxiang 
*.....date:2018/7/10
*..descrip:
//
*/

#include <jni.h>
#include "logUtils.h"
extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/log.h"
}


#undef LOG_TAG
#define LOG_TAG "(>_<)decode(>_<) "
extern "C"{

JNIEXPORT jint JNICALL Java_com_dxtest_simpleastffmpeg_JniFFmpeg02Decoder_decode
        (JNIEnv * env, jobject jobj, jstring jinputurl, jstring joutputurl){



}

}
