package com.dxtest.demolearnffmpeg01;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.PixelFormat;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.AppCompatButton;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.dxtest.simpleastffmpeg.JniFFmpeg02Decoder;

import java.io.File;

public class DecodeActivity extends AppCompatActivity implements View.OnClickListener{

    private static final String TAG = DecodeActivity.class.getSimpleName();

    private TextView tv_show;
    private AppCompatButton tv_input_url;
    private AppCompatButton tv_output_url;
    private JniFFmpeg02Decoder mJniFFmpeg02Decoder;

    private Thread mPlayThread;
    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_decode);

        tv_show= (TextView) findViewById(R.id.tv_show);
        tv_input_url= (AppCompatButton) findViewById(R.id.tv_input_url);
        tv_output_url= (AppCompatButton) findViewById(R.id.tv_output_url);
        mSurfaceView= (SurfaceView) this.findViewById(R.id.surfaceview);
        mSurfaceHolder=mSurfaceView.getHolder();

        //要设置SurfaceView 的显示格式为RGBA_8888，缓存大小为 宽度x高度x4，
        // 而通过FFmpeg解码出来的数据要转换为RGBA数据，缓存大小也是 宽度x高度x4，
        // ################ 而宽度，高度都是固定视频的大小，二者的缓存空间大小一致才不会出错，且排列顺序都为RGBA。##########
        mSurfaceHolder.setFormat(PixelFormat.RGBA_8888);//注意：设置SurfaceView显示的格式为RGBA_8888


        File file=new File("/storage/emulated/0/ddmsrec.mp4");
        tv_input_url.setText(file.getAbsolutePath());
        tv_output_url.setText(file.getParentFile().getAbsolutePath());

        tv_input_url.setOnClickListener(this);
        tv_output_url.setOnClickListener(this);

        mJniFFmpeg02Decoder=new JniFFmpeg02Decoder();

    }


    public void onClick(View v){
        Intent intent=null;
        final String inputurl= tv_input_url.getText().toString();
        String outputurl= tv_output_url.getText().toString();
        switch (v.getId()){
            case R.id.btn_decode:
                Log.i("inputurl",inputurl);
                Log.i("outputurl",outputurl);
                mJniFFmpeg02Decoder.decode(inputurl,outputurl);
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
                        mJniFFmpeg02Decoder.deCodeVideo(inputurl,surface);
                    }
                };
                mPlayThread.start();
                break;
            case R.id.tv_input_url:
                break;
            case R.id.tv_output_url:
                intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("image/*");
                //intent.setType("audio/*"); //选择音频
                //intent.setType("video/*"); //选择视频 （mp4 3gp 是android支持的视频格式）
                //intent.setType("video/*;image/*");//同时选择视频和图片
                intent.addCategory(Intent.CATEGORY_OPENABLE);
                startActivityForResult(intent,1);
                break;
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == Activity.RESULT_OK) {//是否选择，没选择就不会继续

            Uri uri = data.getData();//得到uri，后面就是将uri转化成file的过程。

            String datastr=data.getDataString();
            String data1 = data.getStringExtra("data");
            Log.e(TAG, "DecodeActivity([requestCode, resultCode, data]): datastr="+datastr+";data1 = "+data1);
            File file=new File(uri.getPath());
            if(file.exists()){
                datastr=file.getParentFile().getAbsolutePath();
                tv_input_url.setText(file.getAbsolutePath());
                tv_output_url.setText(datastr);
            }

            String[] proj = {MediaStore.Images.Media.DATA};
            Cursor actualimagecursorOld = getContentResolver().query(uri,proj,null,null,null);
            Cursor actualimagecursor = managedQuery(uri, proj, null, null, null);

            Log.e(TAG, "DecodeActivity([requestCode, resultCode, data]): actualimagecursorOld = "+actualimagecursorOld+";actualimagecursor = "+actualimagecursor);
            if (actualimagecursor!=null){
                int actual_image_column_index = actualimagecursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
                actualimagecursor.moveToFirst();
                String img_path = actualimagecursor.getString(actual_image_column_index);
                File filet = new File(img_path);
                Log.e(TAG, "DecodeActivity([requestCode, resultCode, data]):file = "+filet);
            }

        }
    }

}
