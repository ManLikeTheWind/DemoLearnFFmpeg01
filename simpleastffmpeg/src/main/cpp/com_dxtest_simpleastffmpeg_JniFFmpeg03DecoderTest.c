/*
Administrator="ADSF"
*.fileName: com_dxtest_simpleastffmpeg_JniFFmpeg03DecoderTest.c
*..project:DemoLearnFFmpeg01
*.....user:dongxiang 
*.....date:2018/7/16
*..descrip:
//
*/

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"

#include "com_dxtest_simpleastffmpeg_JniFFmpeg03DecoderTest.h"
#include "logUtils.h"

#undef LOG_TAG
#define LOG_TAG "com_dxtest_simpleastffmpeg_JniFFmpeg03DecoderTest"
/*
 * Class:     com_dxtest_simpleastffmpeg_JniFFmpeg03DecoderTest
 * Method:    deCodeVideo
 * Signature: (Ljava/lang/String;Landroid/view/Surface;)I
 */
JNIEXPORT jint JNICALL Java_com_dxtest_simpleastffmpeg_JniFFmpeg03DecoderTest_deCodeVideo
        (JNIEnv *env, jobject jobj, jstring input_jstr, jobject surface){

    const char* input_path=(*env)->GetStringUTFChars(env,input_jstr,JNI_FALSE);
    // 1.注册所有组件
    av_register_all();
    //2.打开输入文件
    AVFormatContext *pFormatCtx=avformat_alloc_context();
    if( avformat_open_input(&pFormatCtx,input_path,NULL,NULL) ){
        LOGE("open file failure;line = %d;",__LINE__);
        return -1;
    }

    //3.获取视频文件信息；
    if( avformat_find_stream_info(pFormatCtx,NULL)<0 ){
     LOGE("get mediaFile info failure; line = %d;",__LINE__);
        return -1;
    }

    //4.查找编码器
    int video_stream_index=-1;
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if( pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO ){
            video_stream_index=i;
            break;
        }
    }
    if( video_stream_index==-1 ){
        LOGE("not found video stream ;line = %d",__LINE__);
        return -1;
    }

    AVCodecContext *pCodecCtx=pFormatCtx->streams[video_stream_index]->codec;
    AVCodec * pCodec=avcodec_find_decoder(pCodecCtx->codec_id);

    //5.打开编码器
    if( avcodec_open2(pCodecCtx,pCodec,NULL)<0 ){
        LOGE("open encoder failure;");
        return -1;
    }

    ANativeWindow* nativeWindow=ANativeWindow_fromSurface(env,surface);
    ANativeWindow_Buffer outBuffer;
    //6.从输入文件读取数据，
    //6.1 存放解码器前数据；
    AVPacket * pPacket=av_packet_alloc();
    //6.2 存放解码后的数据；
    AVFrame *pFrame=av_frame_alloc();
    //6.3 存放或转换为RGBA后的数据；
    AVFrame *pFrameRGBA=av_frame_alloc();
    //6.4 用于格式转换的SwsContext：
    // 由于解码出来的帧格式不是RGBA的，在渲染之前需要进行格式转换
    struct SwsContext *sws_ctx=sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                              pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGBA,
                                              SWS_BILINEAR, NULL,
                                              NULL, NULL);
    //如果没有帧可以加压缩，则为0，否则不为0；
    int got_picture_ptr;
    int countFrame=0;
    while (av_read_frame(pFormatCtx,pPacket)==0){
        //解码视频
        if( pPacket->stream_index==video_stream_index ){
            //7.解码一帧数据
            if( avcodec_decode_video2(pCodecCtx,pFrame,&got_picture_ptr,pPacket)<0 ){
             LOGE("deocder error ;line = %d;",__LINE__);
            }
            if( got_picture_ptr>=0 ){
                LOGE("解码第%d帧",++countFrame);
                ANativeWindow_setBuffersGeometry(nativeWindow,
                                                 pCodecCtx->width,pCodecCtx->height,
                                                WINDOW_FORMAT_RGBA_8888);
                ANativeWindow_lock(nativeWindow,&outBuffer,NULL);
                av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize,
                                     outBuffer.bits, AV_PIX_FMT_RGBA,
                                    pCodecCtx->width, pCodecCtx->height, 1);

                //格式转换
                //int sws_scale(struct SwsContext *c, const uint8_t *const srcSlice[],
                //              const int srcStride[], int srcSliceY, int srcSliceH,
                //              uint8_t *const dst[], const int dstStride[]);
                sws_scale(sws_ctx,pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height,
                          pFrameRGBA->data, pFrameRGBA->linesize);

                ANativeWindow_unlockAndPost(nativeWindow);
            }
        }
        av_packet_unref(pPacket);
    }

    ANativeWindow_release(nativeWindow);
    av_free(pPacket);
    av_free(pFrame);
    av_free(pFrameRGBA);

    //8.关闭解码器
    avcodec_close(pCodecCtx);
    //9.关闭输入文件；
    avformat_close_input(&pFormatCtx);

    (*env)->ReleaseStringChars(env,input_jstr,input_path);
    return 1;
}
