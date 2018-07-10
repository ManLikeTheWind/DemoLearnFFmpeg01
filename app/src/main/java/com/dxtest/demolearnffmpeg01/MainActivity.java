package com.dxtest.demolearnffmpeg01;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ScrollView;
import android.widget.TextView;

import com.dxtest.simpleastffmpeg.JniFFmpegHelloWorld;


public class MainActivity extends AppCompatActivity {
    private static final String TAG = MainActivity.class.getSimpleName();

    JniFFmpegHelloWorld mJniFFmpegApplicaion;
    TextView mTextView ;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        View child= LayoutInflater.from(this).inflate(R.layout.activity_main,null);
        ScrollView sv=new ScrollView(this);
        ViewGroup.LayoutParams layoutParams = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT);
        sv.addView(child);

        setContentView(sv);
        mJniFFmpegApplicaion=new JniFFmpegHelloWorld();
        // Example of a call to a native method
        mTextView = (TextView) findViewById(R.id.sample_text);
        mTextView.setText(mJniFFmpegApplicaion.stringFromJNI());

        invokeAll(mJniFFmpegApplicaion);


    }

    private void invokeAll(JniFFmpegHelloWorld mJniFFmpegApplicaion) {
        StringBuffer sb=new StringBuffer(mTextView.getText()+"\n\n");
        sb.append("\n urlprotocolinfo = "+mJniFFmpegApplicaion.urlprotocolinfo());
        sb.append("\n avformatinfo = "+mJniFFmpegApplicaion.avformatinfo());
        sb.append("\n avcodecinfo = "+mJniFFmpegApplicaion.avcodecinfo());
        sb.append("\n avfilterinfo = "+mJniFFmpegApplicaion.avfilterinfo());
        sb.append("\n configurationinfo = "+mJniFFmpegApplicaion.configurationinfo());


        mTextView.setText(sb.toString());

    }

}
