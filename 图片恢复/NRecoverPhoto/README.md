Android 图片恢复
===============
- 使用 `gradle-experimental` 插件来开发NDK

    [gradle-experimental插件的更新](https://bintray.com/android/android-tools/com.android.tools.build.gradle-experimental)
    
    使用方法不多介绍请查看[Android JNI](http://xiaoxiaoying.net/jni/2016/08/02/ANDROID_JNI.html)
    
- **手机root的情况下**
- **使用RootTools检查当前应用是否有root权限 / 或者使用 `Runtime.getRuntime().exec("su")`获取Root权限**[RootTools](https://github.com/Stericson/RootTools)
- **获取`data`镜像文件路径**
  <pre>
    获取方式:
    读取文件
    /proc/partitions 或 /proc/mounts
    两个文件随便读取一起中一个
    使用命令查看
    1、adb shell
    2、su
    3、cat /proc/mounts
    <img src="/resources/data_path.png" />
    图中/dev/block/platform/15570000.ufs/by-name/USERDATA就是data的路径
    4、查看镜像路径 ls -l /dev/block/platform/15570000.ufs/by-name/USERDATA
    <img src="/resources/path.png"/>
    /resources/data_path.png就是data的镜像路径
   </pre>
    
_使用C语言扫描 `data` 镜像文件_