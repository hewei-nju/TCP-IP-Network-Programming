/** @author heweibright@gmail.com 
 *  @date 2021/9/3 11:06
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

const int BUF_SIZE = 1024;

void error_handling(const char *msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        std::terminate();
    }

    int acpt_sock, recv_sock;
    struct sockaddr_in acpt_addr;
    if ((acpt_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error_handling("socket() error");

    memset(&acpt_addr, 0, sizeof(acpt_addr));
    acpt_addr.sin_family = PF_INET;
    acpt_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_addr.sin_port = htons(atoi(argv[1]));

    if (bind(acpt_sock, reinterpret_cast<sockaddr *>(&acpt_addr), sizeof(acpt_addr)) == -1)
        error_handling("bind() error");

    if (listen(acpt_sock, 5) == -1)
        error_handling("listen() error");

    if ((recv_sock = accept(acpt_sock, 0, 0)) == -1)
        error_handling("accept() error");
    ssize_t len;
    char buf[BUF_SIZE];
    while ((len = recv(recv_sock, buf, sizeof(buf), MSG_PEEK | MSG_DONTWAIT)) != -1 && len == 0);
    if (len == -1)
        error_handling("recv() error");
    buf[len] = '\0';
    std::cout << "Buffering " << len << " bytes: " << buf << '\n';
    if ((len = recv(recv_sock, buf, sizeof(buf), 0)) == -1)
        error_handling("recv() error");
    buf[len] = '\0';
    std::cout << "Read again: " << buf << '\n';
    close(acpt_sock);
    close(recv_sock);
    return 0;
}