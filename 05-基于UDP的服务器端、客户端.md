# 基于UDP的服务器端、客户端

## 1. UDP内部工作原理

![image-20210829014300176](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210829014300176.png)

* IP：让离开主机B的UDP数据包准确传递到主机A；
* UDP：根据端口号将传到主机的数据包交付给最终的UDP套接字；

### 1. UDP的高效使用

TCP比UDP慢的原因：

1. 收发数据前后进行的连接设置及清除过程；
2. 收发数据过程中为保证可靠性而添加的流控制；

如果收发的数据量小但需要频繁连接时，UDP比TCP高效。

## 2. 实现基于UDP的服务器端、客户端

### 1. UDP中的服务器端和客户端没有连接

UDP服务器端、客户端不需要再连接状态下交换数据。即不用像TCP连接过程中调用的listen函数和accept函数。

UDP中只有创建套接字和数据交换过程；

### 2. UDP服务器端、客户端均只要一个套接字

TCP中套接字时一对一关系，如果要向10个客户端提供服务，则需要提供10个服务器端套接字；而UDP则只需要一个套接字；只要一个UDP套接字就能同时和多台主机通信；

### 3. 基于UDP的数据I/O函数

因为TCP是会建立连接再传输数据，因此TCP套接字必定知道目标地址的信息；

而UDP则不会，因此每次传输数据都要添加目标地址信息；

```c++
ssize_t sendto(int sock, void* buff, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen);
```

* sock：用于传输数据的UDP套接字文件描述符；
* buff：保存待传输数据的缓冲地址；
* nbytes：待传输的数据长度，字节为单位；
* flags：可选参数，如果没有则为0；
* to：存有目标地址信息的sockaddr结构体变量的地址值；
* addrlen：传递给参数to的地址值结构体变量的长度；
* 成功返回发送的数据的长度，失败返回-1；

```c++
ssize_t recvfrom(int sock, void* buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen);
```

* sock：用于接收数据的UDP套接字文件描述符；
* buff：保存接收数据的缓冲地址；
* nbytes：可接收数据的最大长度，字节为单位；
* flags：可选参数，如果没有则为0；
* to：存有发送端地址信息的sockaddr结构体变量的地址值；
* addrlen：保存给参数from的地址值结构体变量的长度；
* 成功返回接收的数据的长度，失败返回-1；

## 3. 基于UDP的回声服务器端、客户端

代码见Code/uecho_server.cpp、Code/uecho_client.cpp。

### 1. UDP客户端套接字地址的分配

TCP客户端调用connect函数能自动完成此过程；

UDP程序中，调用sendto函数传输数据前应该完成对套接字的地址分配工作，因此调用bind函数；

bind函数不区分TCP和UDP。如果调用sendto函数时发现尚未分配地址信息，则在首次调用sendto函数时会给相应的套接字自动分配IP和端口；而此次分配的地址一直保存到程序结束为止；

## 4. UDP数据传输特性和调用connect函数

TCP数据传输过程中不存在边界，表明数据传输过程中调用I/O函数的次数不具有任何意义；

UDP时具有数据边界的协议，因此传输过程中I/O函数的调用次数很重要；也就是说：输入函数调用次数必须得等于输出函数的调用次数，这样才能保证接收全部发送的数据；

### 1. 已连接UDP套接字和未连接UDP套接字

TCP套接字中需要注册待传输数据的目标IP和端口号，UDP中则无需注册；

通过sendto函数传输数据的过程大致分为如下阶段：

1. 向UDP套接字注册目标IP和端口号；
2. 传输数据；
3. 删除UDP套接字中注册的目标地址信息；

向上面这种每次调用sendto函数时更换目标地址，这种未注册目标地址信息的套接字称为**未连接套接字**。

但有时候我们希望通过UDP套接字多次像一个目标地址进行传输数据，那么多次调用sendto时，如果多次注册和删除显然是一种消耗：

```
IP为211.210.147.82的主机82号端口共准备了3个数据，调用3次sendto函数进行传输。
```

### 2. 创建已连接UDP套接字

只需要针对UDP套接字调用connect函数即可：

```c++
sock = socket(PF_INET, SOCK_DGRAM, 0);
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = ...;
addr.sin_port = ...;
connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
```

* 已连接UDP并不是意味着要与对方UDP套接字连接，而实注册目标IP和端口信息；
* 此时已经指定了收发对象，因此还可以使用write、read函数进行通信；