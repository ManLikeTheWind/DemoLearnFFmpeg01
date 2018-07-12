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
#include <android/native_window.h>
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

    const char *input_str =(*env)->GetStringUTFChars(env,inputurl_jstr,NULL);
    const char *output_str=(*env)->GetStringUTFChars(env,outputurl_jstr,NULL);
    //不能使用 字符数组，那个参数里面都是 const ，不然会导致数据返回失败
//    char input_str[500]={0};
//    char output_str[500]={0};
//    sprintf(input_str,"%s",(*env)->GetStringUTFChars(env,inputurl_jstr,JNI_FALSE));
//    sprintf(output_str,"%s",(*env)->GetStringUTFChars(env,outputurl_jstr,NULL));
    char info[1000]={0};
    LOGE("input_str = %s",input_str);
    LOGE("output_str = %s",output_str);

    //FFmpeg av_log() callback
    av_log_set_callback(custom_log);
    av_register_all();
    pFormatCtx=avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx,input_str,NULL,NULL)!=0){
        LOGE("Couldn't open input stream.  input_str = %s;input_path = %s",input_str,input_str);
        return -1;
    }
    LOGE("asdfasdfasfasdfasdf");
    avformat_network_init();

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

    (*env)->ReleaseStringUTFChars(env,inputurl_jstr,input_str);
    (*env)->ReleaseStringUTFChars(env,outputurl_jstr,output_str);

    return 0;

}




JNIEXPORT jint JNICALL Java_com_dxtest_simpleastffmpeg_JniFFmpeg02Decoder_deCodeVideo
        (JNIEnv *env, jobject instance, jstring input_, jobject surface) {
    const char *input_path = (*env)->GetStringUTFChars(env,input_, 0);
    LOGE("input_path = %s",input_path);

    //1st.注册所有组件
    /**
     * 初始化libavformat和注册所有的 muxers, demuxers 和协议，如果你不想使用次函数，
     * 则可以手动选择你想要的支持格式
     * 详情你可以选择下面的函数查询
     * @see av_register_input_format()
     * @see av_register_output_format()
     *
     * muxer是合并将视频文件、音频文件和字幕文件合并为某一个视频格式。如，可将a.avi, a.mp3, a.srt用muxer合并为mkv格式的视频文件。
     * demuxer是拆分这些文件的。
     */
    av_register_all();

    //2nd.打开输入文件
    //int avformat_open_input(AVFormatContext**ps,const char *url, AVInputFormat *fmt, AVDictionary **options);
    //2.1初始化AVFormatContext *pFormatCtx
    // 封装格式上下文结构体，也是统领全局的结构体，保存了视频文件封装格式相关信息。
    AVFormatContext *pFormatCtx=avformat_alloc_context();
    //2.2打开输入文件
    /**
     * 打开输入流并且读取头信息。但解码器没有打开
     * 这个输入流必须使用avformat_close_input()关闭
     * @param ps（第一个参数的形参名称） 指向 你由你提供AVFormatContext（AVFormatContext是由 avformat_alloc_context 函数分配的）。
     *          有可能ps指向空，在这种情况下，AVFormatContext由此函数分配并写入ps。
     *      注意： 你提供的AVFormatContext在函数执行失败的时候将会被释放
     * @param url 你要打开视频文件路径.
     * @param fmt  如果不为空，那么这个参数将强制作为输入格式，否则自动检索
     * @param options 一个关于AVFormatContext and demuxer-private 选项的字典.
     * 返回时，此参数将被销毁，并替换为包含未找到的选项的dict。有可能是空的
     *
     * @return 返回0表示成功, 一个负数常量AVERROR是失败的.
     *
     * @note 如果你想自定义IO,你需要预分配格式内容并且设置pd属性
     */
    if (avformat_open_input(&pFormatCtx,input_path,NULL,NULL)!=0){
        LOGE("开发输入文件失败");
        return -1;
    }
    //上面打开输入流后会将视频封装格式信息写入AVFormatContext中,那么我们下一步就可以得到一些展示信息

    //3rd.获取视频文件信息
     /**
       *
       * 读取媒体文件中的数据包以获取流信息，这个对于文件格式没有头信息的很有帮助，比如说mpeg
       * 这个函数还可以计算在MPEG-2重复帧模式的真实帧速率。
       * 逻辑文件位置不会被这个函数改变
       * 检索过的数据包或许会缓存以供后续处理
       * @param ic  第一个参数 封装格式上下文
       * @param options
       *              如果不为空， 一个长度为 ic.nb_streams （封装格式所有流，字幕 视频 音频等） 的字典。
       *              字典中第i个成员  包含一个对应ic第i个流中对的编码器。
       *              在返回时，每个字典将会填充没有找到的选项
       * @return 如果返回>=0 代表成功, AVERROR_xxx 表示失败
       *
       * @note 这个函数 不保证能打开所有编码器，所以返回一个非空的选项是一个完全正常的行为
       *
       *
       * @todo
       *  下个版本目标无视即可
       * Let the user decide somehow what information is needed so that
       *       we do not waste time getting stuff the user does not need.
       */
    //int avformat_find_stream_info(AVFormatContext *ic,AVDictionary **options);
    if (avformat_find_stream_info(pFormatCtx,NULL)<0){
        LOGE("获取视频文件信息失败");
        return  -1;
    } else{
        //输出视频信息
          LOGI("视频的文件格式：%s",pFormatCtx->iformat->name);
          LOGI("视频时长：%d", (pFormatCtx->duration)/1000000);
    }


    //4th.查找编解码器
    //AVCodec *avcodec_find_decoder(enum AVCodecID id);
    //4.1 遍历所有类型的流（音频流、视频流、字幕流），找到视频流
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
    //4.2 获取视频流的编码器上下文，保存了视频（音频）编解码相关信息
    AVCodecContext *pCodecCtx=pFormatCtx->streams[video_stream_index]->codec;
    //4.3 通过编码器上下文（存放的编码器信息）存放的编码器ID，
    // 每种视频（音频）编解码器(例如H.264解码器)对应一个该结构体。
    AVCodec *pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    //（迅雷看看，找不到解码器，临时下载一个解码器）
    if (pCodec == NULL)  {
        LOGE("找不到解码器\n");
        return -1;
    }
    //5th.代开编码器
     /**
       * 初始化 指定AVCodecContext去使用 给定的AVCodec
       * 在使用之前函数必须使用avcodec_alloc_context3（）分配上下文。
       *
       * 以下函数 avcodec_find_decoder_by_name(), avcodec_find_encoder_by_name(),
       * avcodec_find_decoder() and avcodec_find_encoder() 提供了一个简便的得到一个解码器的方法
       *
       * @warning 这个函数线程不是安全的
       *
       * @note 在使用解码程序之前，始终调用此函数 (如 @ref avcodec_decode_video2()).
       * 下面是示例代码
       * @code
       * avcodec_register_all();
       * av_dict_set(&opts, "b", "2.5M", 0);
       * codec = avcodec_find_decoder(AV_CODEC_ID_H264);
       * if (!codec)
       *     exit(1);
       *
       * context = avcodec_alloc_context3(codec);
       *
       * if (avcodec_open2(context, codec, opts) < 0)
       *     exit(1);
       * @endcode
       *
       *
       * @param avctx 要初始化的编码器
       * @param codec 用这个codec去打开给定的上下文编码器.如果 codec 不为空 那么必须
       * 事先用avcodec_alloc_context3和avcodec_get_context_defaults3传递给这个context，那么这个codec
       * 要么为NULL要么就是上面调用函数所使用的codec
       *
       * @param
       *
       * 选项填充AVCodecContext和编解码器私有选项的字典。返回时，此对象将填充未找到的选项。
       *
       * @return 返回0表示成功， 负数失败
       * @see avcodec_alloc_context3(), avcodec_find_decoder(), avcodec_find_encoder(),
       *      av_dict_set(), av_opt_find().
       */
    //int avcodec_open2(AVCodecContext *avctx,const AVCodec *codec,AVDictonary **options);
    if( avcodec_open2(pCodecCtx,pCodec,NULL)<0 ){
        LOGE("打开编码器失败");
        return -1;
    } else{
        //输出视频信息
        LOGE("视频的文件格式：%s",pFormatCtx->iformat->name);
        //得到视频播放时长
        if(pFormatCtx->duration != AV_NOPTS_VALUE){
            int hours, mins, secs, us;
            int64_t duration = pFormatCtx->duration + 5000;
            secs = duration / AV_TIME_BASE;
            us = duration % AV_TIME_BASE;
            mins = secs / 60;
            secs %= 60;
            hours = mins/ 60;
            mins %= 60;
            LOGE("%02d:%02d:%02d.%02d\n", hours, mins, secs, (100 * us) / AV_TIME_BASE);

        }
        LOGE("视频的宽高：%d,%d",pCodecCtx->width,pCodecCtx->height);
        LOGE("解码器的名称：%s",pCodec->name);
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

    //读取每一帧
    /**
     *返回下一帧的流
     * 此函数返回存储在文件中的内容，并且不会验证解码器有什么有效帧。
     * 函数将存储在文件中的帧进行分割 并且返回给每一个调用者。
     *
     * 函数不会删除在有效帧之间的无效数据 以便在可能解码过程中提供解码器最大的信息帮助
     * 如果 pkt->buf 是空的,那么这个对应数据包是有效的直到下一次调用av_read_frame()
     * 或者直到使用avformat_close_input().否则包无期限有效
     * 在这两种情况下 这个数据包当你不在需要的时候,你必须使用使用av_free_packet释放它
     * 对于视屏,数据包刚好只包含一帧.对于音频,如果它每一帧是一个已知固定大小的,那么他包含整数帧(如. PCM or ADPCM data)
     * 如果音频帧具有可变大小(如. MPEG audio),那么他只包含一帧
     * pkt->pts, pkt->dts and pkt->duration 始终在AVStream.time_base 单位设置正确的数值
     *(如果这个格式无法提供.那么进行猜测)
     * 如果视频格式有B帧那么pkt->pts可以是 AV_NOPTS_VALUE.如果你没有解压他的有效部分那么最好依靠pkt->dts
     *
     * @return 0表示成功, < 0 错误或者文结束
     */
    while( av_read_frame(pFormatCtx,pPacket)==0 ){
        //一个包里有很多种类型如音频视频等 所以判断 这个包对应的流的在封装格式的下表
        //如果这个包是视频频包那么得到压缩的视频包
        if( pPacket->stream_index==video_stream_index ){
            //7th. 解码-帧数据
            //int avcodec_decode_video2(AVCodecContext *avctx,AVFrame *picture,int *got_picture_ptr,const AVPacket *avpkt);
            if( avcodec_decode_video2(pCodecCtx,pFrame,&got_picture_ptr,pPacket)<0 ){
                LOGE("解码错误");
            }
            if( got_picture_ptr>=0 ){
                LOGE("解码第 [%d] 帧",++countFrame);
                ANativeWindow_setBuffersGeometry(nativeWindow,pCodecCtx->width,pCodecCtx->height,WINDOW_FORMAT_RGBA_8888);
                //Y 亮度 UV 色度（压缩了） 人对亮度更加敏感; U V 个数是Y的1/4
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
                //               //获取帧率tbr fbs
                //float fram_rate =pFormatCtx->streams[v_stream_idx]->avg_frame_rate.num/pFormatCtx->streams[v_stream_idx]->avg_frame_rate.den;
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


