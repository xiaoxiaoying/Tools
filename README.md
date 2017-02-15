### 使用说明

#### 1、恢复图片
[图片恢复代码](https://gitlab.huduntech.com/huduntech/AndroidDataRecoveryAppJni/tree/master/%E5%9B%BE%E7%89%87%E6%81%A2%E5%A4%8D)  
其中包括`可执行程序`和`编译为.so文件`  
目的：适配部分手机不能读取文件问题

#### 2、短信回复和通话记录恢复
[demo](https://gitlab.huduntech.com/huduntech/AndroidDataRecoveryAppJni/tree/master/RecoverySMS)  
工具：Android Studio  
使用`gradle-experimental`插件  
编译必须指定： 

<pre>
            moduleName = "recovery"
            ldLibs.addAll(["log", "z", "m"])
            stl = "gnustl_static"
            cppFlags.addAll(["-std=c++11", "-fexceptions", "-O2", "-Wall", "-frtti", "-pie", " -fPIE"])
            abiFilters.addAll(['armeabi', 'armeabi-v7a', 'arm64-v8a',
                               'x86', 'x86_64',
                               'mips', 'mips64'])

</pre>
具体看demo中的配置
