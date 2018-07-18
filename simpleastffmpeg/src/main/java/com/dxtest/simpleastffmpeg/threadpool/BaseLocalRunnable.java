package com.dxtest.simpleastffmpeg.threadpool;

import android.os.Handler;
import android.util.Log;

/**
 * <br>package: com.dxtest.simpleastffmpeg.threadpool  BaseLocalRunnable
 * <br>.author: dongxiang
 * <br>...date: 2018/7/17  10:14
 * <br>.descrp:
 * <br>..using:
 * <br>.e-mail:dongxiang_android_sdk@aliyun.com
 */

public abstract  class BaseLocalRunnable implements LocalRunnable,MainRunnable {
    private static final String TAG = BaseLocalRunnable.class.getSimpleName();
    private Handler mainHandler;
    private MainRunnable mainRunnable;

    int taskid;

    public BaseLocalRunnable(Handler mainHandler, MainRunnable mainRunnable) {
        this.mainHandler = mainHandler;
        this.mainRunnable = mainRunnable;
    }

    public abstract Object dobackground();

    @Override
    public final void run(){
        onfinished(dobackground(),taskid);
    }

    public final void execute(int taskid){
        ThreadPoolRun.instance.addTask(this);
        this.taskid=taskid;
        if( !ThreadPoolRun.instance.runflag ){
            ThreadPoolRun.instance.runTask();
        }
    }

    @Override
    public final void onfinished(final Object object,final int raskid){
        mainHandler.post(new Runnable() {
            @Override
            public void run() {
                if( mainRunnable!=null ){
                    mainRunnable.onfinished(object,raskid);
                }
                Log.e(TAG, "执行完毕一个任务 onfinished . run([]):object "+object+";raskid =  "+raskid+";  开始执行下一个任务");
                //读取下一个信息
                ThreadPoolRun.instance.runend();
            }
        });
    }

    @Override
    public final void before(int id){
        if( mainRunnable!=null ){
         mainRunnable.before(id);
        }
    }


}
