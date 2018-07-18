package com.dxtest.simpleastffmpeg.threadpool;

import android.util.Log;

import com.dxtest.simpleastffmpeg.base.BaseJniFFmpegApplication;

/**
 * <br>package: com.dxtest.simpleastffmpeg.threadpool  ThreadPool
 * <br>.author: dongxiang
 * <br>...date: 2018/7/17  8:43
 * <br>.descrp:线程池
 * <br>..using:
 * <br>.e-mail:dongxiang_android_sdk@aliyun.com
 */

public class ThreadPool extends BaseJniFFmpegApplication {

    private static final String TAG = ThreadPool.class.getSimpleName();
    /**
     * 初始化本地线程池
     */
    public native void init();

    /** 本地线程中执行任务；
     * @param runnable
     * @return
     */
    public native int run(LocalRunnable runnable);

    /**
     * 释放资源
     */
    public native void recycle();


    public static void logStatic(){
        Log.e(TAG, "ThreadPool.logStatic([tag, content]): ");
    }

    public void logNormal(){
        Log.e(TAG, "ThreadPool.logNormal([tag, content]): ");
    }


    public void callBack_JVM(){
        Log.e(TAG, "callBack_JVM([]): ");
    }
}
