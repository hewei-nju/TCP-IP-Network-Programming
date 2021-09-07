# 优于select的epoll

## 1. epoll理解与应用

I/O复用的传统方法由select函数和poll函数。但是由于种种原因，select的性能不太令人满意，因此有了Linux下的epoll。

select函数实现I/O复用的程序，无论怎么优化程序性能都无法同时接入上百个客户端。这使得select不适合作为Web服务器端开发为主流的现代开发环境。

### 1. 基于select的I/O复用技术速度慢的原因

从代码上就很容以发现不合理的设计：

* 调用select函数后常见的针对所有文件描述符的循环语句；
* 每次调用select函数时都要向该函数传递监视对象的值；

因为select函数并不是将发生变化的文件描述符单独集中到一起，而实通过观察作为监视对象的fd_set变量的变化，找到发生变化的文件描述符，因此无法避免针对所有监视对象的循环语句。

并且作为监视对象的fd_set变量会发生变化，所以调用select函数前应该保存原有信息，并在每次调用select函数时传递新的监视对象信息；

**影响提高性能的最大障碍**：每次传递监视对象的信息；

* 传递的监视对象信息具有含义：**每次调用select函数时向操作系统传递监视对象信息**；

应用程序向操作系统传递数据会对程序造成巨大负担，并且无法通过代码优化解决！

**但是为什么要将监视对象信息传递给操作系统**？

* select函数是监视套接字变化的函数，套接字是操作系统管理的资源，因此select函数需要借助于操作系统才能完成其功能；

**通过如下方式弥补**：

* 仅向操作系统传递1次监视对象，监视返回或内容发生变化时只通知变化的事项。

Linux支持上述处理方式是epoll。

### 2. select的优点

只要满足如下条件就应该使用select：

* 服务器端接入者少；
* 程序要具有兼容性；

### 3. 实现epoll时必要的函数和结构体

epoll函数的优点：

* 无需编写以监视状态变化为目的的针对所有文件描述符的循环语句；
* 调用对于select函数的epoll_wait函数时无需每次传递监视对象的信息；

epoll服务器端需要实现的3个函数：

1. epoll_create：创建保存epoll文件描述符的空间；
    * 因为epoll方式下，由操作系统负责保存监视对象文件描述符，因此需要向操作系统请求创建保存文件描述符的空间；
2. epoll_ctl：向空间注册并注销文件描述符；
    * epoll_ctl函数相当于select中使用FD_SET, FD_CLR函数；
3. epoll_wait：等待文件描述符发生变化；
    * select通过fd_set变量查看监视对象的状态变化；epoll通过epoll_event将发生变化的文件描述符集中到一起；

```c++
struct epoll_event {
    __uint32_t events;
    epoll_data_t data;
}

typedef union epoll_data {
    void * ptr;
    int fd;
    __uint_t u32;
    __uint64 u64;
} epoll_data_t;
```

#### 1. epoll_create

> * epoll是Linux2.5.44版内核引入，因此需要检验Linux内核版本，确保是2.6以上；使用`cat /proc/sys/kernel/osrelease`;

```c++
#include <sys/epoll.h>
int epoll_create(int size);
```

* size：epoll实例的大小的参考；实际由操作系统决定！Linux2.6.8后内核完全忽略了传入的size参数；
* 成功返回epoll文件描述符，失败返回-1；

epoll_create函数创建的文件描述符保存空间称为**epoll例程**；

epoll_create创建的资源同套接字相同，由操作系统管理；并且也会返回文件描述符，该文件描述符主要用于区分epoll历程。需要终止时，也要调用close函数；

#### 2. epoll_ctl

```c++
#include <sys/epoll.h>
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```

* epfd：用于注册监视对象的epoll例程的文件描述符；
* op：用于指定监视对象的添加、删除或更改等操作；
    * EPOLL_CTL_ADD：将文件描述符注册到epoll例程；
    * EPOLL_CTL_DEL：从epoll例程中删除文件描述符；
    * EPOLL_CTL_MOD：更改注册的文件描述符的关注事件发生情况；
* fd：需要注册的监视对象文件描述符；
* event：监视对象的事件类型；
* 成功返回0，失败-1；

```c++
// epoll例程A中注册文件描述符B，目的是监视参数C中的事件
epoll_ctl(A, EPOLL_CTL_ADD, B, C);

// 从epoll例程A中删除文件描述符B
epoll_ctl(A, EPOLL_CTL_DEL, B, NULL);
```

> Linux2.6.9前：epoll_ctl最后一个参数不能传递NULL，即使没用！

* epoll_event结构体用于保存发生事件的文件描述符集合；也可以在epoll例程中注册文件描述符时，用于注册关注的事件；

```c++
struct epoll_event event;
...
event.events = EPOLLIN;  // 发生需要读取数据的情况(事件)时
event.data.fd = sockfd;
epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
...
```

* 上述代码将sockfd注册到epoll例程epfd中，并在需要读取数据的情况下产生相应事件；
* epoll_event的成员events中可以保存的常量及所指的事件类型：
    * EPOLLIN：需要读取数据的情况；
    * EPOLLOUT：输出缓冲为空，可以立即发送数据的情况；
    * EPOLLPRI：收到OOB数据的情况；
    * EPOLLRDHUB：断开连接或半关闭的情况，在边缘出发方式下很有用；
    * EPOLLERR：发生错误的情况下；
    * EPOLLET：以边缘触发的方式得到事件通知；
    * EPOLLONESHOT：发生一次事件后，相应文件描述符不再收到事件通知；因此需要向epoll_ctl第二个参数传递EPOLL_CTL_MOD，再次设置事件；

#### 3. epoll_wait

```c++
#include <sys/epoll.h>
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

* epfd：表示事件发生监视范围的epoll例程的文件描述符；
* events：保存发生事件的文件描述符集合的结构体地址值；
* maxevents：第二个参数中可以保存的最大事件数；
* timeout：以1/1000秒为单位的等待事件，传递-1时一直等待直到发生事件；
* 成功返回发生事件的文件描述符数，失败返回-1；

第二个参数所指向的缓冲需要动态分配：

```c++
int event_cnt;
struct epoll_event *ep_events;
...
ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);  // EPOLL_SIZE为常量
...
event = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
...
```

使用实例见Code/echo_epollserv.cpp;

## 2. 条件触发和边缘触发

Level Trigger：条件触发；

Edge Trigger：边缘触发；

### 1. 条件触发和边缘触发的区别在于事件的时间点

条件触发原理：条件触发方式中，只要某事件对应的内容存在就会一直通知该事件；

* 比如输入缓冲中收到50字节数据，服务器端系统将通知该事件；服务器端读取20字节后还剩下30字节的情况下仍然会注册该事件；

边缘触发原理：当某事件内容存在时会通知一次该事件，后续将不再通知；

### 2. 掌握条件触发的事件特性

使用实例见Code/echo_EPLTserv.cpp;

看上面代码可以发现它与Code/echo_epollserv.cpp的区别很小，目的就是为了展示条件触发的事件特性！

epoll默认就是条件触发的！

### 3. 掌握边缘触发的事件特性

使用实例见Code/echo_EPETserv.cpp;

通过对比Code/echo_EPETserv.cpp和Code/echo_EPLT.cpp可以发现二者区别也很小，而且运行时服务端确实复合边缘触发的事件特性！

但是客户端运行出现了问题！客户端不能正常地进行输出！

* 当客户端输入的字符数小于等于服务端一次性能读入的字符数时，能够正常回显；
* 当客户端输入的字符数大于服务端一次性能读入的字符数时，将不能够正常回显，进入阻塞状态；

**原因**：

* 因为服务器端采用边缘触发，对于客户端输入的字符数多余服务端能一次性读取的字符数时，第一次会读取服务器端允许读入的字符数，然后就不会在有事件触发，即使缓冲区还有数据可读，因此会进入阻塞状态！

### 4. 边缘触发的服务器端实现中必知的两点

* 通过errno变量验证错误原因；
* 为了完成非阻塞I/O，更改套接字特性；

Linux套接字相关函数通过返回-1通知发生了错误，然后通过头文件< error.h >全局变量errno来指明具体的错误；

Linux提供的更改或读取文件属性的方法如下：

```c++
#include <fcntl.h>
int fcntl(int filedes, int cmd, ...);
```

* filedes：属性更改目标的文件描述符；
* cmd：表示函数调用的目的；
* 成功返回cmd参数相关值，失败返回-1；

```c++
int flag = fcntl(fd, F_GETFL, 0);  // 获取fd的属性
fcntl(fd, F_SETFL, flag|O_NONBLOCK);  // 将fd改为非阻塞模式
```

调用read & write函数时，无论是否存在数据都会形成非阻塞文件。

### 5. 实现边缘触发的回声服务器端

实例见Code/echo_EPETserv2.cpp;

既然边缘触发方式中，接收数据时仅注册1次该事件，即一旦发生输入相关事件，就应该读取输入缓冲中的所有数据。故需要验证输入缓冲是否为空：

```
read函数返回-1，errno中值为EAGAIN时，说明无数据可读；
```

这样，**为何还要将套接字设位非阻塞的**？

* 边缘触发方式下，以阻塞方式工作的read & write函数可能引起服务器端的长时间停顿，因为如果这次没有把数据全部读写完(如读写缓冲区太小)，那么下次调用epoll_wait()时，它不会通知你，也就是它只会通知你一次，直到该文件描述符上出现第二次可读写事件才会通知你！因此一定要采用非阻塞read & write函数。

### 6. 条件触发和边缘触发的优劣

边缘触发的优点：

* 可以分离接收数据和处理数据的时间点；

![image-20210905193730310](https://cdn.jsdelivr.net/gh/hewei-nju/PictureBed@main/img/image-20210905193730310.png)

* 服务器从A，B，C接收数据，然后按照A、B、C的顺序将信息组合；
* 组合的数据被发送给任意的主机；
* 如果出现：B，C在工作，A还尚未连接，则此时A、B、C乱序发送数据；

因此，即使输入缓冲收到数据，服务器端也能决定读取和处理这些数据的时间点，这给服务器端的实现带来了很大的灵活性。

条件触发虽然也能解决上述问题，但是：如果不读取，则每次调用epoll_wait函数时都会产生相应的事件，服务器能接受吗？大概率是不行的！