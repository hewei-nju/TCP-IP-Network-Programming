/** @author heweibright@gmail.com 
 *  @date 2021/9/2 6:37
 *  Copyright (c) All rights reserved.
 */


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

const int BUF_SIZE = 1024;

void error_handling(const char *msg) {
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage : " << argv[0] << " <port>\n";
        std::terminate();
    }

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    char buf[BUF_SIZE];
    struct timeval timeout;
    fd_set reads, cpy_reads;
    int fd_max, str_len, fd_num;

    /** hhhh，估计不少小伙伴和我有一样的疑惑吧！为什么这里要将serv_sock这个文件描述符加入检测？
     * 这就要理解网络之间的连接请求的简单关系！下面是我的片面的理解！
     * 1. 首先是服务其运行中，等待客户端发来请求！在socket函数中展示为listen；
     * 2. 然后是客户端，通过IP和port向服务器去发送请求，以便获取某些服务；
     * 3. 客户端去连接时，也会将自己的IP，port等数据信息传递过去；这就表现为服务器端的文件描述符有内容可读；
     *
     * 其实我只是按照计网以及自己对前面内容的简单理解这么猜测的！于是我便做了一个测试在源代码的注释部分！
     * 在客户端connect前，是不会读取任何东西；connect后，便可以读到一个SOH(ASCII = 1):
     * 注意：采用调试的方式，查看buf中的内容！
     * */
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;
    while (true) {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;

        if (fd_num == 0)
            continue;

        for (int i = 0; i < fd_max + 1; i++) {
            if (FD_ISSET(i, &cpy_reads)) {
                if (i == serv_sock) {
                    //=== 测试serv_sock可以读取client的port和IP等内容
                    // read(serv_sock, buf, BUF_SIZE);
                    // std::cout << buf << std::endl;  // SOH：Start Of Header
                    //===
                    if ((clnt_sock = accept(serv_sock, 0, 0)) == -1)
                        error_handling("accept() error");

                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    std::cout << "connected client: " << clnt_sock << '\n';
                } else {
                    if ((str_len = read(i, buf, BUF_SIZE)) == -1)
                        error_handling("read() error");
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        std::cout << "closed client: " << i << '\n';
                    } else {
                        if (write(i, buf, str_len) == -1)
                            error_handling("write() error");
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}
