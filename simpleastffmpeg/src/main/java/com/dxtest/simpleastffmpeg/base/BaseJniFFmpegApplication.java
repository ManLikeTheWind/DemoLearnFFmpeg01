package com.dxtest.simpleastffmpeg.base;

/**
 * <br>package: com.dxtest.simpleastffmpeg.base  BaseJniFFmpegApplication
 * <br>.author: dongxiang
 * <br>...date: 2018/7/10  12:47
 * <br>.descrp:
 * <br>..using:
 * <br>.e-mail:dongxiang_android_sdk@aliyun.com
 */

public class BaseJniFFmpegApplication {

    // Used to load the 'ffmpeg-dev' library on application startup.
    static {
        System.loadLibrary("ffmpeg-dev");
    }
}
