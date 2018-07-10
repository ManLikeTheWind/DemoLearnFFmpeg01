package com.dxtest.simpleastffmpeg;

import com.dxtest.simpleastffmpeg.base.BaseJniFFmpegApplication;

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
 *  Protocol:  FFmpeg类库支持的协议
 *  AVFormat:  FFmpeg类库支持的封装格式
 *  AVCodec:   FFmpeg类库支持的编解码器
 *  AVFilter:  FFmpeg类库支持的滤镜
 *  Configure: FFmpeg类库的配置信息
 *
 * This is the simplest program based on FFmpeg in Android. It can show following
 * informations about FFmpeg library:
 *  Protocol:  Protocols supported by FFmpeg.
 *  AVFormat:  Container format supported by FFmpeg.
 *  AVCodec:   Encoder/Decoder supported by FFmpeg.
 *  AVFilter:  Filters supported by FFmpeg.
 *  Configure: configure information of FFmpeg.
 *
 *
 *
 */
public class JniFFmpegHelloWorld extends BaseJniFFmpegApplication{
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**  Protocol:  FFmpeg类库支持的协议 */
    public native String urlprotocolinfo();

    /**  AVFormat:  FFmpeg类库支持的封装格式 */
    public native String avformatinfo();

    /**  AVCodec:   FFmpeg类库支持的编解码器 */
    public native String avcodecinfo();

    /**  AVFilter:  FFmpeg类库支持的滤镜 */
    public native String avfilterinfo();

    /**  Configure: FFmpeg类库的配置信息 */
    public native String configurationinfo();


}
