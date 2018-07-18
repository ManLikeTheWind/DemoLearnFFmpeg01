package com.dxtest.demolearnffmpeg01.temp;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.dxtest.demolearnffmpeg01.R;
import com.dxtest.simpleastffmpeg.threadpool.BaseLocalRunnable;
import com.dxtest.simpleastffmpeg.threadpool.MainRunnable;

public class Demo2PThreadActivity extends AppCompatActivity  implements MainRunnable{
    private Handler mHandler;
    private static final String TAG = Demo2PThreadActivity.class.getSimpleName();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_demo2_pthread);

        mHandler=new Handler();

    }
    int i=0;
    public void onClick (View view){
        switch (view.getId()){
            case R.id.btn:
                Task task=new Task(mHandler,this);
                task.execute(++i);
                Log.e(TAG, " onClick  currentThread = "+Thread.currentThread());
                new Thread(){
                    @Override
                    public void run() {
                        Log.e(TAG, " onClick 2  currentThread = "+Thread.currentThread());
                    }
                }.start();
                break;
        }

        return ;
    }

    @Override
    public void before(int id) { Log.v("任务", id + "开启");

    }

    @Override
    public void onfinished(Object object, int raskid) {
        Log.v("结果", "结束" + object);
    }
    //子线程方法
    static class Task extends BaseLocalRunnable {

        public Task(Handler mainHandler, MainRunnable mainRunnable) {
            super(mainHandler, mainRunnable);
        }

        @Override
        public Object dobackground() {
                StringBuffer sb=new StringBuffer(":\n");
            for (int i = 0; i < 10; i++) {
                sb.append("looper-num = "+i+"\n");
            }
            Log.e(TAG," dobackground  currentThread = "+ Thread.currentThread()+";sb = "+sb);
            return sb;
        }
    }
}
