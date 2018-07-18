package com.dxtest.simpleastffmpeg.threadpool;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

/**
 * <br>package: com.dxtest.simpleastffmpeg.threadpool  TestJava
 * <br>.author: dongxiang
 * <br>...date: 2018/7/17  10:29
 * <br>.descrp:
 * <br>..using:
 * <br>.e-mail:dongxiang_android_sdk@aliyun.com
 */

public class TestJava implements MainRunnable{
    Handler handler=null;

    public  void main(String[]args){
        Looper.prepare();
        handler=new Handler();
        Task task=new Task(handler,this);
        task.execute(0);
        Looper.loop();


    }
    static class Task extends BaseLocalRunnable{

        public Task(Handler mainHandler, MainRunnable mainRunnable) {
            super(mainHandler, mainRunnable);
        }

        @Override
        public Object dobackground() {
            return null;
        }
    }


    @Override
    public void before(int id) { Log.v("任务", id + "开启");

    }

    @Override
    public void onfinished(Object object, int raskid) {
        Log.v("结果", "结束" + object);
    }
}
