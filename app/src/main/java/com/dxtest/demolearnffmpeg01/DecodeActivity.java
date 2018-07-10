package com.dxtest.demolearnffmpeg01;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.AppCompatButton;
import android.util.Log;
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
    JniFFmpeg02Decoder mJniFFmpeg02Decoder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_decode);

        tv_show= (TextView) findViewById(R.id.tv_show);
        tv_input_url= (AppCompatButton) findViewById(R.id.tv_input_url);
        tv_output_url= (AppCompatButton) findViewById(R.id.tv_output_url);
        tv_input_url.setOnClickListener(this);
        tv_output_url.setOnClickListener(this);
        mJniFFmpeg02Decoder=new JniFFmpeg02Decoder();

    }


    public void onClick(View v){
        Intent intent=null;
        switch (v.getId()){
            case R.id.btn_decode:
                String folderurl= Environment.getExternalStorageDirectory().getPath();

                String urltext_input= tv_input_url.getText().toString();
                String inputurl=folderurl+"/"+urltext_input;

                String urltext_output= tv_output_url.getText().toString();
                String outputurl=folderurl+"/"+urltext_output;

                Log.i("inputurl",inputurl);
                Log.i("outputurl",outputurl);

                mJniFFmpeg02Decoder.decode(inputurl,outputurl);
                break;
            case R.id.tv_input_url:
                intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("*/*");//设置类型，我这里是任意类型，任意后缀的可以这样写。
                intent.addCategory(Intent.CATEGORY_OPENABLE);
                startActivityForResult(intent,1);
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

            String datastr=data.getDataString();
            String data1 = data.getStringExtra("data");
            Log.e(TAG, "DecodeActivity([requestCode, resultCode, data]): datastr="+datastr+";data1 = "+data1);


            Uri uri = data.getData();//得到uri，后面就是将uri转化成file的过程。
            String[] proj = {MediaStore.Images.Media.DATA};
            Cursor actualimagecursorOld = getContentResolver().query(uri,proj,null,null,null);
            Cursor actualimagecursor = managedQuery(uri, proj, null, null, null);

            Log.e(TAG, "DecodeActivity([requestCode, resultCode, data]): actualimagecursorOld = "+actualimagecursorOld+";actualimagecursor = "+actualimagecursor);
            if (actualimagecursor!=null){
                int actual_image_column_index = actualimagecursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
                actualimagecursor.moveToFirst();
                String img_path = actualimagecursor.getString(actual_image_column_index);
                File file = new File(img_path);
                Log.e(TAG, "DecodeActivity([requestCode, resultCode, data]):file = "+file);
            }

        }
    }

}
