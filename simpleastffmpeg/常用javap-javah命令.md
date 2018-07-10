#常用javac javap javah 命令

##javac
***（编译工具）***
<br>
```
[ options ] [ sourcefiles ]

参数可以任意 排列
options：

    -g 生成所有调试信息
    -g:none 不生成任何调试信息
    -g:{lines,vars,source} 只生成某些调试信息
    -nowarn 不生成任何警告
    -verbose 输出有关编译器正在执行的操作的消息
    -deprecation 输出使用已过时的API的源位置
    -classpath <路径> 指定查找用户类文件的注释处理程序的位置
    -cp <路径> 指定查找用户类文件和注释处理程序的位置
    -sourcepath <路径> 指定查找输入源文件的位置
    -bootclasspath <路径> 覆盖引导类文件的位置
    -extdirs <目录> 覆盖所安装扩展的位置
    -endorseddirs <目录> 覆盖签名的标准路径的位置
    -d <目录> 指定放置生成的类文件的位置
    -s <目录> 指定放置生成的源文件的位置
    -version 版本信息
    -Werror 出现警告时终止程序
```
命令行选项。

```sourcefiles
    一个或多个要编译的源文件（例如 MyClass.java）
```

##javap
***（反编译工具）***
<br>
```这个主要用来查看生成JAVA方法的签名，以便于C++中调用类中的方法```
<br>
``` javap [options] [classes]

    常用options

        -version 版本信息
        -v -verbose 打印附加信息(堆栈大小、各方法的 locals 及 args 数,以及class文件的编译版本)
        -l 输出行及局部变量表
        -public 只显示 public 类及成员
        -protected 只显示 protected 和 public 类及成员
        -package 只显示包、protected 和 public 类及成员。这是缺省设置
        -private 显示所有类和成员
        -s 输出内部类型签名(主要使用这个参数查看签名)
        -classpath[路径] 指定 javap 用来查找类的路径。如果设置了该选项，则它将覆盖缺省值或 CLASSPATH 环境变量。目录用冒号分隔

    [classes]

        要反编译的类名
 ```
 <p>
 <b>运行指令</b>
 E:\2018-4-3_dongxiang\workspace\DemoLearnFFmpeg01\simpleastffmpeg\build\intermediates\classes\debug\com\dxtest\simpleastffmpeg\> javap -s -l JniFFmpegHelloWorld.class


 </p>


<br>***运行结果图：***
<div width="150" height="150"  align=center>
    <img width="150" height="150" src="/readme-src/javap.png"/>
</div>


##javah
***（自动生成JNI头文件）***
<br>
<br>......javah 生成实现本地方法所需的 C 头文件和源文件。
<br>C 程序用生成的头文件和源文件在本地源代码中引用某一对象的实例变量。
<br>.h 文件含有一个 struct 定义，该定义的布局与相应类的布局平行。
<br>该 struct 中的域对应于类中的实例变量。
```
javah [options] [classes]

[options]

    -o <文件> 输出文件(只能使用 -d 或 -o 之一)
    -d <目录> 输出目录
    -v -verbose 启用详细输出
    -version 输出版本信息
    -jni 生成JNI样式的标头文件(默认值，可以不写)
    -force 始终写入输出文件
    -classpath <路径> 从中加载类的路径
    -bootclasspath <路径> 从中加载引导类的路径

[classes]

是使用其全限定名称指定的，例如 java.lang.Object.MyClass

```

***运行指令：***
E:\2018-4-3_dongxiang\workspace\DemoLearnFFmpeg01\simpleastffmpeg\src\main\java>  javah  com.dxtest.simpleastffmpeg.JniFFmpegHelloWorld
E:\2018-4-3_dongxiang\workspace\DemoLearnFFmpeg01\simpleastffmpeg\src\main\java>  javah -classpath ./ -jni com.dxtest.simpleastffmpeg.JniFFmpegHelloWorld


