package com.dxtest.demolearnffmpeg01.temp;

import android.graphics.PixelFormat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.AppCompatButton;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;

import com.dxtest.demolearnffmpeg01.R;
import com.dxtest.simpleastffmpeg.JniFFmpeg02Decoder;
import com.dxtest.simpleastffmpeg.JniFFmpeg03DecoderTest;

import java.io.File;

public class Demo1DecodeActivity extends AppCompatActivity {
    private TextView tv_show;
    private AppCompatButton tv_input_url;
    private AppCompatButton tv_output_url;

    private Thread mPlayThread;
    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;

    JniFFmpeg03DecoderTest mJniFFmpeg02DecoderTest;

    File file;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_demo1_decode);

        tv_show= (TextView) findViewById(R.id.tv_show);
        tv_input_url= (AppCompatButton) findViewById(R.id.tv_input_url);
        tv_output_url= (AppCompatButton) findViewById(R.id.tv_output_url);

        mSurfaceView= (SurfaceView) this.findViewById(R.id.surfaceview);
        mSurfaceHolder=mSurfaceView.getHolder();
        //要设置SurfaceView 的显示格式为RGBA_8888，缓存大小为 宽度x高度x4，
        // 而通过FFmpeg解码出来的数据要转换为RGBA数据，缓存大小也是 宽度x高度x4，
        // ################ 而宽度，高度都是固定视频的大小，二者的缓存空间大小一致才不会出错，且排列顺序都为RGBA。##########
        mSurfaceHolder.setFormat(PixelFormat.RGBA_8888);//注意：设置SurfaceView显示的格式为RGBA_8888


        file=new File("/storage/emulated/0/ddmsrec.mp4");
        tv_input_url.setText(file.getAbsolutePath());
        tv_output_url.setText(file.getParentFile().getAbsolutePath());


        mJniFFmpeg02DecoderTest=new JniFFmpeg03DecoderTest();
    }


    public void onClick(View view){
        switch (view.getId()){
            case R.id.btn_decode:
                //是原视频的储存地址
                String selectedAviFile=file.getAbsolutePath();
                //压缩后的视频的储存地址
                String compressedFile=file.getAbsolutePath();
        String cmds= "ffmpeg -y -i "
                    + selectedAviFile
                    + " -strict -2 -vcodec libx264 -preset ultrafast -crf 20 -acodec copy -ar 44100 -ac 2 -b:a 96k -vf scale=-1:800 "
                    + compressedFile;

                break;
            case R.id.btn_play:
                if (mPlayThread!=null){
                    mPlayThread.interrupt();
                    mPlayThread=null;
                }
                mPlayThread=new Thread(){
                    @Override
                    public void run() {
                        Surface surface=mSurfaceHolder.getSurface();
                        mJniFFmpeg02DecoderTest.deCodeVideo(file.getAbsolutePath(),surface);
                    }
                };
                mPlayThread.start();
                break;
        }
    }
}
