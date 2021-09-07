/** @author heweibright@gmail.com 
 *  @date 2021/9/2 16:55
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

void error_handling(const char* msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <por>\n";
        std::terminate();
    }

    int sock;
    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
        error_handling("connect() error");

    if (write(sock, "123", strlen("123")) == -1)
        error_handling("write() error");
    if (send(sock, "4", strlen("4"), MSG_OOB) == -1)
        error_handling("send() error");
    if (write(sock, "567", strlen("567")) == -1)
        error_handling("write() error");
    if (send(sock, "890", strlen("890"), MSG_OOB) == -1)
        error_handling("send() error");

    close(sock);
    return 0;
}
