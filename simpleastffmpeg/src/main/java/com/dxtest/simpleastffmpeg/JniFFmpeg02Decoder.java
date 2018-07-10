package com.dxtest.simpleastffmpeg;

import com.dxtest.simpleastffmpeg.base.BaseJniFFmpegApplication;

/**
 * <br>package: com.dxtest.simpleastffmpeg  JniFFmpeg02Decoder
 * <br>.author: dongxiang
 * <br>...date: 2018/7/10  16:07
 * <br>.descrp:
 * <br>..using:
 * <br>.e-mail:dongxiang_android_sdk@aliyun.com
 */

public class JniFFmpeg02Decoder extends BaseJniFFmpegApplication {

    public native int decode(String inputurl,String outputurl);


}
