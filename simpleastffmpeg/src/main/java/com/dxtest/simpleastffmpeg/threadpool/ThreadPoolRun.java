package com.dxtest.simpleastffmpeg.threadpool;

import android.util.Log;

import java.util.concurrent.LinkedBlockingDeque;

/**
 * <br>package: com.dxtest.simpleastffmpeg.threadpool  ThreadPoolRun
 * <br>.author: dongxiang
 * <br>...date: 2018/7/17  10:13
 * <br>.descrp:
 * <br>..using:
 * <br>.e-mail:dongxiang_android_sdk@aliyun.com
 */

public class ThreadPoolRun {
    private static final String TAG = ThreadPoolRun.class.getSimpleName();
    /** 利用阻塞队列保存任务 */
    private static LinkedBlockingDeque<BaseLocalRunnable> runnables=new LinkedBlockingDeque<>();
    private BaseLocalRunnable lr=null;
    public static ThreadPoolRun instance=new ThreadPoolRun();
    private  ThreadPool pool=null;
    boolean runflag=false;

    public ThreadPoolRun() {
        this.pool = new ThreadPool();
        pool.init();
    }

    /**添加任务*/
    public void addTask(BaseLocalRunnable runnable){
        runnables.addLast(runnable);
    }

    /** 取出任务 */
    private BaseLocalRunnable getTask(){
        try {
            if( runnables.size()> 0){
                return runnables.removeFirst();
            }else {
                Log.e(TAG, "getTask([]): 没有任务了，回收native ");
                pool.recycle();
                runflag=false;
                return null;
            }
        }catch (Exception e){
            e.printStackTrace();
            return null;
        }
    }

    public void runTask(){
        synchronized (pool){
            lr=getTask();
            if( lr!=null&&!runflag ){
                runflag=true;
                lr.before(lr.taskid);
                int run = pool.run(lr);
                Log.e(TAG, "runTask([]): run = "+run);
            }
        }
    }

    /** 当每次执行完成上一个任务后，设置标示，运行下一个任务*/
    public void runend(){
        runflag=false;
        runTask();
    }

}
