# 《TCP/IP网络编程》(韩-尹圣雨)学习笔记

<img src="https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/notes.svg" alt="notes" style="zoom:15%;" /><img src="https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/code.svg" alt="code" style="zoom:15%;" />

《TCP/IP网络编程》学习笔记及具体代码实现，代码部分请参考本仓库Code下的源码。如果本笔记的内容对你有用，请点击一个 `star` ，转载请注明出处，谢谢。

----

## 1. :beginner:为什么开放这个仓库？

我本人在学习Linux C/C++系统编程，从《TCP/IP网络编程》这本书入手，获取的资料里包含了书本里的所有源代码！

但是在实际参考时，发现很多代码并不能正常运行！部分代码的错误也很明显，我从网上查找了很多资料、代码，但是实际效率非常的低！并且发现GitHub上很多仓库的代码也是同我从电子书中获取的源代码一摸一样！甚至错误没有修正。

因为是新手入门，走了很多弯路！也希望后来者可以少点弯路。

## 2. 对于仓库中代码的部分说明

* 整个仓库都是基于Linux下的socket编程部分！对于windows部分，请参考其他的学习资料；

* 除了原书中第14章**多播和广播**部分的代码，我没能在我机器上正常运行外，其余代码均可正常运行！
    * 对于多播部分代码：我采用127.0.0.1的IP运行时，客户端和服务器端均正常！但采用多播IP则不能！这说明代码部分应该没有太大问题，因此我猜测是因为我只在同一台机器上运行而导致的(计网学的太垃圾了！暴风哭泣)。广播部分代码同多播差别不大，因此我认为原因同上！
    * TODO：找出错误，并修正。
* 对于24章部分的web服务器：注意服务器运行在Linux环境上，我采用的是Windows下CLion连接本地虚拟机远程开发的模式，因此对于`http://ip:port/index.html`应该使用虚拟机上的浏览器打开！替代方式是使用wget命令`wget http://ip:port/index.html`;
* 我有对代码进行一些细节调整和修改，使得更加直观、复合我学习过程中的想法；
* 代码中可能还存有小问题，欢迎小伙伴`star、fork、pr`；

## 3. :currency_exchange:开发环境

采用Windows下CLion连接本地虚拟机的远程开发模式；

### 1. 虚拟机部分：

**虚拟机**：CentOS 7.2；

**gcc**：4.8.5；

**g++**：4.8.5；

**gdb**：8.3.1；// 为了使用remote debug！CLion会在tool chain说明推荐；

**cmake**：2.8.12.2；

**gdbserver**：用来进行远程调试；// 后来通过映射后，似乎发现可以不用；

### 2. 本地环境

**IDE**：CLion；// 中途也使用过VSCode，不过个人喜欢CLion；

**SSH**：Putty；

**Windows下CLion远程开发**：https://blog.csdn.net/q1009020096/article/details/87715273 // 这个建议参考一部分

**远程调试配置参考**：https://blog.csdn.net/lihao21/article/details/87425187 // 同上

**CLion官方教程**：https://www.jetbrains.com/help/clion/remote-development.html

