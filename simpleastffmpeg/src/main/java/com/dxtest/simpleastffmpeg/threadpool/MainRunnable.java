package com.dxtest.simpleastffmpeg.threadpool;

/**
 * <br>package: com.dxtest.simpleastffmpeg.threadpool  MainRunnable
 * <br>.author: dongxiang
 * <br>...date: 2018/7/17  10:15
 * <br>.descrp:
 * <br>..using:
 * <br>.e-mail:dongxiang_android_sdk@aliyun.com
 */

public interface MainRunnable {

    public void before(int id);

    void onfinished(Object object,int raskid);


}
