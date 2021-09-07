/** @author heweibright@gmail.com 
 *  @date 2021/8/29 2:07
 *  Copyright (c) All rights reserved.
 */


#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>

const int BUF_SIZE = 1024;

void error_handling(const char *msg) {
    std::cerr << msg << "\n";
    std::terminate();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        std::terminate();
    }

    int sock;
    sockaddr_in serv_addr, clnt_addr;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    char message[BUF_SIZE];
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    while (true) {
        ssize_t len = recvfrom(sock, message, BUF_SIZE, 0,
                               reinterpret_cast<sockaddr *>(&clnt_addr), &clnt_addr_len);
        sendto(sock, message, len, 0,
               reinterpret_cast<sockaddr *>(&clnt_addr), clnt_addr_len);
    }

    close(sock);
}
