package com.dxtest.simpleastffmpeg;

import android.view.Surface;

import com.dxtest.simpleastffmpeg.base.BaseJniFFmpegApplication;

/**
 * <br>package: com.dxtest.simpleastffmpeg  JniFFmpeg03DecoderTest
 * <br>.author: dongxiang
 * <br>...date: 2018/7/16  10:16
 * <br>.descrp:
 * <br>..using:
 * <br>.e-mail:dongxiang_android_sdk@aliyun.com
 */

public class JniFFmpeg03DecoderTest extends BaseJniFFmpegApplication {

    public native int decode(String inputurl,String outputurl);

    public native int deCodeVideo(String input, Surface surface);
}
