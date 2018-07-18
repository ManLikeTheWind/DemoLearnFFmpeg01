#include <jni.h>
#include <stdio.h>
#include <malloc.h>
#include "logUtils.h"
#include "com_dxtest_simpleastffmpeg_threadpool_ThreadPool.h"

#undef LOG_TAG
#define LOG_TAG "com_dxtest_simpleastffmpeg_threadpool_ThreadPool"

static JavaVM  *jvm=NULL;
static jobject  jobj_callback=NULL;
static jmethodID  mid=NULL;
static int flag=-1;
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

//在加载库时执行；
 jint JNI_OnLoad(JavaVM *vm,void *reserved){
    jvm=vm;
    LOGE("------- JNI_OnLoad -------");
    return JNI_VERSION_1_6;
}
//在卸载库时执行
 void JNI_OnUnload(JavaVM *vm,void *reserved){
    jvm=NULL;
    LOGE("------- JNI_OnUnload -------");
}
//pthread中执行的函数
static void *nativeWork(void*args){
    LOGE(" ==================== pre nativeWork ==================== ");
    JNIEnv *env=NULL;
    //附加到当前线程从JVM中取出JNIEnv, C/C++从子线程中直接回到Java里的方法时  必须经过这个步骤
    if( (*jvm)->AttachCurrentThread(jvm,&env,NULL)==0 ){
        while (flag==0){
            if( jobj_callback==NULL ){
                //进入等待
                pthread_cond_wait(&cond,&mutex);
            } else{
                //回调java中的线程
                (*env)->CallVoidMethod(env,jobj_callback,mid);
                (*env)->DeleteGlobalRef(env,jobj_callback);
                jobj_callback=NULL;
            }
        }
        //完毕-脱离当前线程
        (*jvm)->DetachCurrentThread(jvm);
    }
    LOGE(" ==================== end nativeWork ==================== ");
    return (void*)1;
}

int *get_memory(int n){
    int *p, i;
    if ((p = (int *)malloc(n * sizeof(int))) == NULL)    {
        LOGE("malloc error\n");
        return p;
    }
    //快速对 p 起始地址，n+sof-int 快速赋值 0；内存初始化一个方法：memery-set
    memset(p, 0, n * sizeof(int));
    for (i = 0; i < n; i++){
        p[i] = i+1;
    }
    return p;
}


/*
 * Class:     com_dxtest_simpleastffmpeg_threadpool_ThreadPool
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_dxtest_simpleastffmpeg_threadpool_ThreadPool_init
        (JNIEnv *env, jobject jobj){
    LOGE("------- Java_com_dxtest_simpleastffmpeg_threadpool_ThreadPool_init -------");
    if( mid==NULL ){
     //缓存方法
        jclass cls=(*env)->FindClass(env,"com/dxtest/simpleastffmpeg/threadpool/LocalRunnable");
        mid=(*env)->GetMethodID(env,cls,"run","()V");
        if( mid==NULL ){
            return;
        }
        flag=0;
        pthread_t threadNew;
        //创建新线程
        pthread_create(&threadNew,NULL,nativeWork,NULL);
    }

    int n=5, *p, i;
    if ((p = get_memory(n)) == NULL){
        return ;
    }
    for (i = 0; i < n; i++){
        LOGE("%d ", p[i]);
    }
    LOGE("\n");
    free(p);
    p = NULL;
}

/*
 * Class:     com_dxtest_simpleastffmpeg_threadpool_ThreadPool
 * Method:    run
 * Signature: (Lcom/dxtest/simpleastffmpeg/threadpool/LocalRunnable;)I
 */
JNIEXPORT jint JNICALL Java_com_dxtest_simpleastffmpeg_threadpool_ThreadPool_run
        (JNIEnv *env, jobject jobj, jobject callback){
    LOGE("------- Java_com_dxtest_simpleastffmpeg_threadpool_ThreadPool_run -------");
        //设置参数，唤醒等待的线程；
    flag=0;
    jobj_callback=(*env)->NewGlobalRef(env,callback);
    return pthread_cond_signal(&cond);
}

/*
 * Class:     com_dxtest_simpleastffmpeg_threadpool_ThreadPool
 * Method:    recycle
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_dxtest_simpleastffmpeg_threadpool_ThreadPool_recycle
        (JNIEnv *env, jobject jobj){
    LOGE("------- pre Java_com_dxtest_simpleastffmpeg_threadpool_ThreadPool_recycle -------");
    //释放资源
    flag=-1;
    if( jobj_callback!=NULL ){
        LOGE("pre");
        (*env)->DeleteGlobalRef(env,jobj_callback);
        jobj_callback=NULL;
        LOGE("aft");
    }
//    else{
//        pthread_cond_signal(&cond);
//    }
    LOGE("------- aft Java_com_dxtest_simpleastffmpeg_threadpool_ThreadPool_recycle -------");
}