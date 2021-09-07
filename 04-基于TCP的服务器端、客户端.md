# 基于TCP的服务器端、客户端

## 1. 实现基于TCP的服务器端、客户端

### 1. TCP服务器端的默认函数调用顺序

```
socket()  		// 创建套接字
	|
bind()			// 分配套接字地址
	|
listen()		// 等待连接请求状态
	|
accept() 		// 允许连接
	|
read()/write()	// 数据交换
	|
close()			// 断开连接
```

### 2. TCP客户端的默认函数调用顺序

```c++
socket()  		// 创建套接字
	|
connect()		// 请求连接
	|
read()/write()	// 数据交换
	|
close()			// 断开连接
```

* 客户端的IP地址和端口在调用connect函数时自动分配；

### 3. 基于TCP的服务端、客户端函数调用关系

![image-20210828200829189](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210828200829189.png)

流程如下：

* 服务端创建套接字后，连续调用bind、listen函数进入等待状态；
* 客户端通过connect函数发起连接请求；
* 客户端只能等到服务端调用listen函数后才能调用connect函数；
* 客户端调用connect函数前，服务器端有可能率先调用accept，此时服务器端在调用accept函数时进入阻塞状态，知道客户端调用connect函数为止；

## 2. 实现迭代服务器端、客户端

![image-20210828202232906](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210828202232906.png)

具体代码见Code/echo_server.cpp、Code/echo_client.cpp；

代码中存在的可能的问题：

```c++
write(sock, message, strlen(message));
int len = read(sock, message, BUF_SIZE - 1);
message[len] = '\0';
```

它每次调用read、write都是以字符串为单位执行实际的I/O操作；

因为TCP不存在数据边界，客户端也是基于TCP，因此多次write函数传递的字符串可能一次性传递到服务器端。还要考虑字符串过长，需要分2个数据包发送。

## 3. 回声客户端的完美实现

由上面客户端中可能存在问题的客户端代码可以发现：如果等待的时间长一些，调用read函数可以一次性读取所有的字符串数据。但这个时间并不确定，可能很长！

但是理想的客户端应该能够收到字符串数据后立即读取并输出！

**解决方案**：因为可以确定接收数据的大小；若之前传输了多少数据，则在接收时循环调用read函数读取多少数据即可。

修改后部分的代码：

```c++
int len = write(sock, message, strlen(message));
int recv_len = 0;
while (recv_len < len) {
    int cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
    if (cnt == -1)
        error_handling("read() error");
    recv_len += cnt;
}
message[recv_len] = '\0';
```

* 注意上述循环的条件，如果改成recv_len != len，在发生异常情况使得recv_len > len时，将会无限循环！

## 4. TCP原理

### 1. TCP套接字中的I/O缓冲

![image-20210829004711977](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210829004711977.png)

* I/O缓冲在每个TCP套接字中单独存在；
* I/O缓冲在创建套接字时自动生成；
* 即使关闭套接字也会继续传递输出缓冲中遗留的数据；
* 关闭套接字将丢失输入缓冲区中的数据；

write函数和windows的send函数并不会在完成向对方主机的数据传输时返回，而实在数据移到输出缓冲时。

但是TCP会保证对输出缓冲数据的传输，所以说write函数在数据传输完成时返回。

### 2. TCP内部工作原理1：与对方套接字连接

3次握手！

![image-20210829010121031](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210829010121031.png)

1. 与对方套接字建立连接；
2. 与对方套接字进行数据交换；
3. 断开与对方套接字的连接；

套接字以**全双工**方式工作；

### 3. TCP内部工作原理2：与对方主机的数据交换

![image-20210829005907872](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210829005907872.png)

* 如果发生传输错误和超时，会进行重传；

### 4.TCP内部工作原理2：断开与套接字的连接

4次握手！

![image-20210829010052976](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210829010052976.png)