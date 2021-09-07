/** @author heweibright@gmail.com 
 *  @date 2021/9/4 16:52
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

// 注意receiver的BUF_SIZE至少要等于sender的BUF_SIZE，否则由于发送的UDP包以及读取速度等原因，会让recvfrom进入阻塞状态！
const int BUF_SIZE = 2048;

void error_handling(const char *msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        std::terminate();
    }

    int recv_sock;
    struct sockaddr_in addr;
    if ((recv_sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
        error_handling("socket() error");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));
    if (bind(recv_sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
        error_handling("bind() error");

    char buf[BUF_SIZE];
    ssize_t len;
    while (true) {
        len = recvfrom(recv_sock, buf, sizeof(buf) - 1, 0, 0, 0);
        if (len < 0)
            break;
        buf[len] = '\0';
        std::cout << buf << std::flush;
    }
    std::cout << '\n';
    close(recv_sock);
    return 0;
}
