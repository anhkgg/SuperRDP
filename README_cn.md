# SuperRDP

[SuperRDP2](https://raw.githubusercontent.com/anhkgg/SuperRDP/main/bin/SuperRDP2.zip)来了。

**自动支持新版远程桌面， 欢迎使用！**

[English](README.md)

众所周知，Windows家庭版无法使用远程桌面，`RDP Wrapper Library`就是用于恢复家庭版该部分阉割的功能。

本项目是基于[rdpwrap](https://github.com/stascorp/rdpwrap)修改的，在此感谢[stascorp](https://github.com/stascorp)的无私的工作。

rdpwrap主要包括安装器和服务dll。

原始版本安装器采用的Delphi(一门比较久远现在很少使用的语言)编写的，作为一个安全从业人员的本能，并不是非常信任使用别人编译的二进制文件。

另外下载版本无法在windows新版中（原版已经2年没有更新了）使用，本着探索技术本真的初心，我觉得研究一下rdpwrap的工作原理。

**所以用C语言重写了安装器，然后优化了服务模块的代码，最终产生了本项目。**

经过研究，弄明白了rdpwrap的工作原理，原来需要对远程桌面服务模块（termsrv.dll)做patch，让其恢复专业版等拥有的功能。

本项目主要重写了安装器，也就是现在SuperRDP.exe，然后重构了rdpwrap模块的patch和hook相关代码。

另外termsrv.dll在不同版本中修复patch的位置和信息也会不同，所以需要持续更新配置文件中的信息。

由于我目前对远程桌面功能重度依赖，并且系统一直是正版Windows家庭版，所以会一直持续更新对新版本的功能支持。

所以，如果大家有需要，欢迎关注（star）并使用。

**使用方法：**

**如果安装中，遇到安全软件提醒，请允许，否则会失败！**

1. 可直接下载release发布文件，运行SuperRDP.exe(需管理员权限)
2. 根据提示选择1（安装）或者2（卸载）
3. 等待完成即可

```
--------------------------------------------------------
---------SuperRDP for Windows 10 Home Version-----------
-------------Copyright (c) 2021 anhkgg.com--------------
-------------anhkgg | 公众号：汉客儿 -------------------
--------------------------------------------------------

--------------------------------------------------------

[+] SuperRDP initialize...

[*] SuperRDP already installed? 【Yes!】

[+] SuperRDP initialize success...

--------------------------------------------------------

Please select option:
    1: Install SuperRDP to Program Files folder (default)
    2: Uninstall SuperRDP
    3: Force restart Terminal Services

>
```

也可以直接使用`SuperRDP_update.bat`进行重新安装。

**验证远程桌面服务是否启用成功的方法：**

1. Win+R，输入mstsc.exe启动远程桌面程序
2. 输入127.0.0.1，连接成功基本验证服务启用成功
3. 或者也可以使用原版的RDPCheck.exe进行验证

**如何升级：**

常规情况下，一般都只需要更新rdpwrap.ini即可，所以：

1. 手工将rdpwarp.ini拷贝到system32目录
2. 或者运行SuperRDP.exe，先选择2卸载，再选择1安装

# update

如果你发现SuperRDP不支持你的版本，你可以通过[github issues](https://github.com/anhkgg/SuperRDP/issues)上传c:\windows\system32\termsrv.dll，我会尽快更新支持。

[查看更新](update.md)


rdpwrap.ini中patch信息在10.0.18362.657(termsrv.dll)之后的版本都是由我加入，经过长时间实战验证，请放心使用。

**应该是兼容大部分老版本的，如果没有相应系统版本信息，可以联系我更新支持。**

**注意**：仅保证对64位系统的支持（毕竟现在很少用32位系统的了吧）

# Support me

如果觉得对你有帮助，请我喝杯咖啡。

![img](pay.png)