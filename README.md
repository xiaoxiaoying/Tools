### Tools

#### 1、恢复图片
[图片恢复代码](https://github.com/xiaoxiaoying/Tools/tree/master/%E5%9B%BE%E7%89%87%E6%81%A2%E5%A4%8D)  
其中包括`可执行程序`和`编译为.so文件`  
目的：适配部分手机不能读取文件问题

#### 2、短信回复和通话记录恢复
[demo](https://github.com/xiaoxiaoying/Tools/tree/master/RecoverySMS)  
工具：Android Studio  
使用`gradle-experimental`插件  
编译必须指定： 

```java
            moduleName = "recovery"
            ldLibs.addAll(["log", "z", "m"])
            stl = "gnustl_static"
            cppFlags.addAll(["-std=c++11", "-fexceptions", "-O2", "-Wall", "-frtti", "-pie", " -fPIE"])
            abiFilters.addAll(['armeabi', 'armeabi-v7a', 'arm64-v8a',
                               'x86', 'x86_64',
                               'mips', 'mips64'])

```
具体看demo中的配置

#### Exif 读取
C/C++ : https://github.com/mkttanabe/exif https://github.com/twitter-university/Exify

js: https://github.com/exif-js/exif-js
