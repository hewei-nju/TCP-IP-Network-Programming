/** @author heweibright@gmail.com 
 *  @date 2021/9/4 20:25
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

const int BUF_SIZE = 1024;

void error_handling(const char *msg) {
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout <<"Usage: " << argv[0] << " <IP> <port>\n";
        std::terminate();
    }

    int sock;
    struct sockaddr_in serv_adr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        std::cout << "Connected...........\n";

    FILE *readfp;
    FILE *writefp;
    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w");
    if (readfp == nullptr || writefp == nullptr)
        error_handling("fdopen() error");

    char message[BUF_SIZE];
    while (true) {
        std::cout <<"Input message(Q to quit): ";
        if (std::cin.peek() == '\n')
            std::cin.get();
        std::cin.get(message, BUF_SIZE - 1, '\n');
        if (!strcmp(message, "q\0") || !strcmp(message, "Q\0"))
            break;
        // 适配fgets要求
        message[strlen(message)] = '\n';
        fputs(message, writefp);
        fflush(writefp);
        fgets(message, BUF_SIZE, readfp);
        std::cout << "Message from server: " << message << '\n';
    }
    fclose(writefp);
    fclose(readfp);
    return 0;
}