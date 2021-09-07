/** @author heweibright@gmail.com 
 *  @date 2021/8/28 21:01
 *  Copyright (c) All rights reserved.
 */
 

#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <cstdlib>

const int BUF_SIZE = 1024;
void error_handling(const char *msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <port>\n";
        std::terminate();
    }

    int sock;
    sockaddr_in serv_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    else
        std::cout << "Connected..........\n";

    char message[BUF_SIZE];
    while (true) {
        std::cout << "Input message(Q to quit): ";

        if (std::cin.peek() == '\n')
            std::cin.get();
        std::cin.get(message, BUF_SIZE, '\n');  // std::cin.get(message, BUF_SIZE);

        if (!(strcmp(message, "q\0") != 0 && strcmp(message, "Q\0") != 0)) {
            break;
        }
        /** 有小问题的代码
        write(sock, message, strlen(message));
        int len = read(sock, message, BUF_SIZE - 1);
        message[len] = '\0';
        */
        ssize_t len = write(sock, message, strlen(message));
        if (len == -1)
            error_handling("write() error");
        ssize_t recv_len = 0;
        while (recv_len < len) {
            ssize_t cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
            if (cnt == -1)
                error_handling("read() error");
            recv_len += cnt;
        }
        message[recv_len] = '\0';
        std::cout << "Message from server: " << message << '\n';
    }
    close(sock);
    return 0;
}

