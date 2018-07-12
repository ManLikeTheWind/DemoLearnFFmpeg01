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
#include <stdio.h>
#include <time.h>
#include "logUtils.h"
//#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/log.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"


#undef LOG_TAG
#define LOG_TAG "(>_<)decode(>_<) "

//OutPut FFmpeg's av_log()
void custom_log(void *ptr,int level,const char*fmt,va_list vl){
    FILE *fp=fopen("/storage/emulated/0/av_log.txt","a+");
    if (fp){
        vfprintf(fp,fmt,vl);
        fflush(fp);
        fclose(fp);
    }

}

JNIEXPORT jint JNICALL Java_com_dxtest_simpleastffmpeg_JniFFmpeg02Decoder_decode
        (JNIEnv * env, jobject jobj, jstring inputurl_jstr, jstring outputurl_jstr){
    AVFormatContext *pFormatCtx;
    int i,videoindex;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pFrame,*pFrameYUV;

    uint8_t  *out_buffer;
    AVPacket *packet;
    int y_size;
    int ret,got_picture;
    struct  SwsContext *img_convert_ctx;
    FILE *fp_yuv;
    int frame_cnt;
    clock_t time_start,time_finish;
    double time_duration = 0.0;

    char input_str[500]={0};
    char output_str[500]={0};
    char info[1000]={0};
    sprintf(input_str,"%s",(*env)->GetStringUTFChars(env,inputurl_jstr,JNI_FALSE));
    sprintf(output_str,"%s",(*env)->GetStringUTFChars(env,outputurl_jstr,NULL));
    LOGE("input_str = %s",input_str);
    LOGE("output_str = %s",output_str);

    //FFmpeg av_log() callback
    av_log_set_callback(custom_log);
    av_register_all();
    avformat_network_init();
    pFormatCtx=avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx,input_str,NULL,NULL)!=0){
        LOGE("Couldn't open input stream.\n");
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx,NULL)<0){
        LOGE("Couldn't find stream information.\n");
        return -1;
    }

    videoindex=-1;
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[1]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoindex=i;
            break;
        }
    }

    if (videoindex==-1){
        LOGE("Couldn't find a video stream.\n");
        return -1;
    }

    pCodecCtx=pFormatCtx->streams[videoindex]->codec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec==NULL){
        LOGE("Couldn't find Codec.\n");
    }
    if (avcodec_open2(pCodecCtx,pCodec,NULL)<0){
        LOGE("Couldn't open codec.\n");
        return -1;
    }


    pFrame=av_frame_alloc();
    pFrameYUV=av_frame_alloc();
    out_buffer= (unsigned char *) av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
    av_image_fill_arrays(pFrameYUV->data,pFrameYUV->linesize,out_buffer,AV_PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height,1);

    packet= (AVPacket *) av_malloc(sizeof(AVPacket));

    img_convert_ctx=sws_getContext(pCodecCtx->width,pCodecCtx->height,
                                   pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height ,
                                   AV_PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);

    sprintf(info,  "[Input      ]%s\n",input_str);
    sprintf(info,"%s[Output     ]%s\n",info,output_str);
    sprintf(info,"%s[Format     ]%s\n",info,pFormatCtx->iformat->name);
    sprintf(info,"%s[Codec      ]%s\n",info,pCodecCtx->codec->name);
    sprintf(info,"%s[Resolution ]%dx%d\n",info,pCodecCtx->width,pCodecCtx->height);


    fp_yuv=fopen(output_str,"wb+");
    if (fp_yuv==NULL){
        LOGE("Cannot open output file.\n");
        return -1;
    }

    frame_cnt=0;
    time_start=clock();
    while (av_read_frame(pFormatCtx,packet)>0){
        if (packet->stream_index==videoindex){
            ret=avcodec_decode_video2(pCodecCtx,pFrame,&got_picture,packet);
            if (ret<0){
                LOGE("Decode Error.\n");
                return -1;
            }
            if (got_picture){
                sws_scale(img_convert_ctx,
                          (const uint8_t* const*)pFrame->data,
                          pFrame->linesize,
                          0,
                          pCodecCtx->height,
                          pFrameYUV->data,
                          pFrameYUV->linesize);

                y_size=pCodecCtx->width*pCodecCtx->height;
                fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);//Y
                fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);//U
                fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);//V

                //Output info
                char pictype_str[10]={0};
                switch(pFrame->pict_type){
                    case AV_PICTURE_TYPE_I:
                        sprintf(pictype_str,"I");
                        break;
                    case AV_PICTURE_TYPE_P:
                        sprintf(pictype_str,"P");
                        break;
                    case AV_PICTURE_TYPE_B:
                        sprintf(pictype_str,"B");
                        break;
                    default:
                        sprintf(pictype_str,"Other");
                        break;
                }
                LOGE("Frame Index: %5d. type:%s ",frame_cnt,pictype_str);
                frame_cnt++;
            }
        }
        av_free_packet(packet);
    }

    //flush decoder
    //Fix: Flush Frames remained in Codec
    while (1){
        ret=avcodec_decode_video2(pCodecCtx,pFrame,&got_picture,packet);
        if (ret<0){
            break;
        }
        if (!got_picture){
            break;
        }

        sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                  pFrameYUV->data, pFrameYUV->linesize);
        int y_size=pCodecCtx->width*pCodecCtx->height;
        fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y
        fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
        fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
        //Output info
        char pictype_str[10]={0};
        switch(pFrame->pict_type){
            case AV_PICTURE_TYPE_I:
                sprintf(pictype_str,"I");
                break;
            case AV_PICTURE_TYPE_P:
                sprintf(pictype_str,"P");
                break;
            case AV_PICTURE_TYPE_B:
                sprintf(pictype_str,"B");
                break;
            default:
                sprintf(pictype_str,"Other");
                break;
        }
        LOGI("Frame Index: %5d. Type:%s",frame_cnt,pictype_str);
        frame_cnt++;
    }
    time_finish = clock();
    time_duration=(double)(time_finish - time_start);
    sprintf(info, "%s[Time      ]%fms\n",info,time_duration);
    sprintf(info, "%s[Count     ]%d\n",info,frame_cnt);
    sws_freeContext(img_convert_ctx);

    fclose(fp_yuv);

    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    return 0;

}




JNIEXPORT jint JNICALL Java_com_dxtest_simpleastffmpeg_JniFFmpeg02Decoder_deCodeVideo
        (JNIEnv *env, jobject instance, jstring input_, jobject surface) {
    const char *input_path = (*env)->GetStringUTFChars(env,input_, 0);
    //1st.注册所有组件
    av_register_all();

    //2nd.打开输入文件
    //int avformat_open_input(AVFormatContext**ps,const char *url, AVInputFormat *fmt, AVDictionary **options);
    //2.1初始化AVFormatContext *pFormatCtx
    AVFormatContext *pFormatCtx=avformat_alloc_context();
    //2.2打开输入文件
    if (avformat_open_input(&pFormatCtx,input_path,NULL,NULL)!=0){
        LOGE("开发输入文件失败");
        return -1;
    }

    //3rd.获取视频文件信息
    //int avformat_find_stream_info(AVFormatContext *ic,AVDictionary **options);
    if (avformat_find_stream_info(pFormatCtx,NULL)<0){
        LOGE("获取视频文件嘻嘻失败");
        return  -1;
    }

    //4th.查找编解码器
    //AVCodec *avcodec_find_decoder(enum AVCodecID id);
    //4.1 获取视频流的索引-下标-位置
    int video_stream_index=-1;//存放视频流的索引（下标）位置
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if( pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO ){
            video_stream_index=i;
            break;
        }
    }
    if( video_stream_index==-1 ){
        LOGE("没找到视频流");
        return -1;
    }
    //4.2 获取视频流的编码器上下文（保存了视频或音频编码器的信息）
    AVCodecContext *pCodecCtx=pFormatCtx->streams[video_stream_index]->codec;
    //4.3 通过编码器上下文（存放的编码器信息）存放的编码器ID获取编码器；
    AVCodec *pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    //（迅雷看看，找不到解码器，临时下载一个解码器）
    if (pCodec == NULL)  {
        LOGE("找不到解码器\n");
        return -1;
    }
    //5th.代开编码器
    //int avcodec_open2(AVCodecContext *avctx,const AVCodec *codec,AVDictonary **options);
    if( avcodec_open2(pCodecCtx,pCodec,NULL)<0 ){
        LOGE("打开编码器失败");
        return -1;
    }

    //6th.从输入文件读取数据（循环读取），av_read_frame只能读取1帧
    //int av_read_frame(AVFormatContext *s,AVPacket *pkt);
    //6.1初始化 AVPacket *pPacket 存放编码前数据；
    AVPacket *pPacket=av_packet_alloc();
    //6.2初始化 AVFrame *pFrame 存放编码后的数据；
    AVFrame *pFrame=av_frame_alloc();
    //6.3初始化 AVFrame *pFrameRGBA 存放转换为RGBA后的数据
    AVFrame *pFrameRGBA=av_frame_alloc();
    //6.4初始化 用于格式转换的SWSContext，由于编码出来的帧格式不是RGBA的，在渲染之前要进行格式转化
//    struct SwsContext *sws_getContext(
//            int srcW,int srcH,enum AVPixelFormat srcFormat,
//            int dstW,int dstH,enum AVPixelFormat dstFormat,
//            int flags,SwsFilter *srcFilter,
//            SwsFilter *dstFilter,const double *param);
    struct SwsContext *sws_ctx=sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,
                                        pCodecCtx->width,pCodecCtx->height,AV_PIX_FMT_RGBA,
                                        SWS_BILINEAR,NULL,
                                        NULL,NULL);
    int got_picture_ptr;//如果没有帧可以解压缩，则为零，否则为非零
    int countFrame=0;

    //返回和java surface关联的ANativeWindow通过本地本地方法交互
    ANativeWindow* nativeWindow=ANativeWindow_fromSurface(env,surface);
    ANativeWindow_Buffer outBuffer;

    while( av_read_frame(pFormatCtx,pPacket)==0 ){
        //解码视频
        if( pPacket->stream_index==video_stream_index ){
            //7th. 解码-帧数据
            //int avcodec_decode_video2(AVCodecContext *avctx,AVFrame *picture,int *got_picture_ptr,const AVPacket *avpkt);
            if( avcodec_decode_video2(pCodecCtx,pFrame,&got_picture_ptr,pPacket)<0 ){
                LOGE("解码错误");
            }
            if( got_picture_ptr>=0 ){
                LOGE("解码第 [%d] 帧",++countFrame);
                ANativeWindow_setBuffersGeometry(nativeWindow,pCodecCtx->width,pCodecCtx->height,WINDOW_FORMAT_RGBA_8888);
                ANativeWindow_lock(nativeWindow,&outBuffer,NULL);
                av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize,
                                     (const uint8_t *) outBuffer.bits, AV_PIX_FMT_RGBA,
                                     pCodecCtx->width, pCodecCtx->height, 1);
                //格式转换
//              int sws_scale(struct SwsContext *c,const uint8_t *const srcSlice[],
//                            const int srcStride[],int srcSliceY,int srcSliceH,
//                            uint8_t *const dist[],const int dstStride[]);
                sws_scale(sws_ctx, (const uint8_t *const *) pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height,
                          pFrameRGBA->data, pFrameRGBA->linesize);

                //unlock
                ANativeWindow_unlockAndPost(nativeWindow);
            }
        }

        av_packet_unref(pPacket);
    }
    ANativeWindow_release(nativeWindow);
    av_free(pPacket);
    av_free(pFrame);
    av_free(pFrameRGBA);
    sws_freeContext(sws_ctx);
    //8th. 关闭解码器
    avcodec_close(pCodecCtx);

    //9.th 关闭输入文件
    avformat_close_input(&pFormatCtx);

    (*env)->ReleaseStringUTFChars(env,input_,input_path);//释放

}


