///*
//Administrator="ADSF"
//*.fileName: demo-pthread.c
//*..project:DemoLearnFFmpeg01
//*.....user:dongxiang
//*.....date:2018/7/17
//*..descrip:
////
//*/
//#include <jni.h>
//#include <sys/types.h>
//#include <pthread.h>
//#include <malloc.h>
//#include "logUtils.h"
//
//#undef LOG_TAG
//#define LOG_TAG "com_dxtest_simpleastffmpeg_threadpool_ThreadPool"
//
//
//static JavaVM  *_javaVM;
//static pthread_mutex_t *_pthread_mutex=PTHREAD_MUTEX_INITIALIZER;
//static pthread_cond_t *_pthread_cond=PTHREAD_COND_INITIALIZER;
//static jboolean flag=JNI_FALSE;
//
//static jmethodID  _methodID;
//static jobject _java_callback_obj;
//
//
//void* native_run(void * argvs){
//    JNIEnv *env;
//    if( (*_javaVM)->AttachCurrentThread(_javaVM,&env,NULL)==0 ){
//        while (flag){
//            if( _java_callback_obj==NULL ){
//                pthread_cond_wait(&_pthread_cond,&_pthread_mutex);
//            } else{
//                (*env)->CallVoidMethod(env,_java_callback_obj,_methodID,NULL);
//                (*env)->DeleteGlobalRef(env,_java_callback_obj);
//            }
//        }
//        (*_javaVM)->DetachCurrentThread(_javaVM);
//    }
//    return (void*)1;
//
//}
//
//static void main(JNIEnv *env,jobject jobj){
//    if( _methodID==NULL ){
//        (*env)->GetJavaVM(env,&_javaVM);
//
//        jclass cls=(*env)->FindClass(env,"com/dxtest/simpleastffmpeg/threadpool/LocalRunnable");
//        _methodID=(*env)->GetMethodID(env,cls,"run","()V;");
//        if( _methodID==NULL ){
//            return;
//        }
//        flag=JNI_TRUE;
//        pthread_t  _thread;
//        //创建线程并执行；
//        pthread_create(&_thread,NULL,native_run,NULL);
//    }
//}
//
//static void main2(JNIEnv *env,jobject jobj,jobject java_callobj){
//    flag=JNI_TRUE;
//    _java_callback_obj=(*env)->NewGlobalRef(env,java_callobj);
//    pthread_cond_signal(&_pthread_cond);
//}
//
//static void main3(JNIEnv *env,jobject jobj,jobjectArray cmd_line){
//
//    //java 数组转C语言数组；
//    int i=0;//满足ndk c99 标准
//    char**argv=NULL;//命令集合 二维指针
//    jstring *strr=NULL;
//    int cmdnum=0;
//    if( cmd_line!=NULL ){
//        cmdnum=(*env)->GetArrayLength(env,cmd_line);
//        argv=malloc(sizeof(char*)*cmdnum);
//        strr=malloc(sizeof(jstring)*cmdnum);
//
//        for (int i = 0; i < cmdnum; ++i) {//转换
//            *(strr+1)=(*env)->GetObjectArrayElement(env,cmd_line,i);
//            argv=(*env)->GetStringUTFChars(env,*(strr+i),JNI_FALSE);
//        }
//    }
//    size_t s;
//
//    free(strr);
//
//}
//
//  struct Student{
//    int num;
//    char* name;
//}Tom,*Lily;
//
//void main4(){
//    int n=10;
//    //1.对于Tom形式：不需对结构体分配内存；但是需要对name 分配内存---并释放内存；
//    Tom.name=malloc(sizeof(char)*n);
//    free(Tom.name);
//
//    Lily=malloc(sizeof(struct Student));
//    Lily->name=malloc(sizeof(char)*n);
//    free(Lily->name);
//    free(Lily);
//
//}
//
//
//
//
//
//
//
//
//
//
