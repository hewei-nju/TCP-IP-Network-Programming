# 制作HTTP服务器端

## 1. HTTP概要

Web服务器端的一种理解：

* 基于HTTP协议，将网页对应文件传输给客户端的服务器端！

HTTP协议：基于TCP/IP实现的以超文本传输为目的的而设计的应用层协议；

### 1. HTTP

**无状态的Stateless协议**：

![image-20210906163545851](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210906163545851.png)

* 服务器端相应客户端请求后立即断开连接；因此叫无状态的stateless协议；

> 为了弥补HTTP无法保持连接的缺点，Web编程中通常会使用Cookie和Session技术。

### 2. 请求消息(Request Message)的结构

客户端向服务端发送请求消息，Web服务器端需要解析并响应客户请求：

![image-20210906163929629](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210906163929629.png)

* GET /index.html HTTP/1.1：以GET方式请求index.html文件，以1.1版本的HTTP协议进行通信；
* 请求行只能以一行发送；
* 消息头：包含发送请求的浏览器信息、用户认证信息等；

### 3. 响应消息(Response Message)的结构

Web服务器端向客户端传递的响应信息的结构：

![image-20210906164832533](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210906164832533.png)

* HTTP/1.1 200 ok：使用HTTP1.1版本进行响应，请求已经正确处理；
* 200 OK：成功处理请求；
* 404 Not Found：请求的文件不存在；
* 400 Bad Request：请求方式错误，请检查；
* 消息头：含有传输的数据类型和长度等信息；

## 2. 实现简单的Web服务器端

实例见Code/webserv.cpp; html文件为index.html;

**注意**：

* 我是通过windows的Clion进行远程开发的！
* 因此程序实际运行在虚拟机中，所以直接在windows下通过web或其他工具是无法访问到该服务的；
* 所以直接去虚拟机上用浏览器查看或者使用wget也行！

