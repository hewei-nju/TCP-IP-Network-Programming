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

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <port>\n";
        std::terminate();
    }

    int sock;
    sockaddr_in serv_addr, clnt_addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(clnt_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    char message[BUF_SIZE];
    socklen_t clnt_addr_len;
    while (true) {
        std::cout << "Input message(Q to quit): ";
        std::cin >> message;
        if (!strcmp(message, "q\0") || !strcmp(message, "Q\0")) {
            break;
        }
        sendto(sock, message, strlen(message), 0, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr));
        ssize_t len = recvfrom(sock, message, BUF_SIZE, 0, reinterpret_cast<sockaddr *>(&clnt_addr), &clnt_addr_len);
        message[len] = '\0';
        std::cout << "Message from server: " << message << '\n';
    }
    close(sock);
    return 0;
}
