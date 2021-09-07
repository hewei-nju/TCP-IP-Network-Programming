/** @author heweibright@gmail.com 
 *  @date 2021/9/4 20:18
 *  Copyright (c) All rights reserved.
 */


#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>

const int BUF_SIZE = 1024;
void error_handling(const char* msg)
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

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    char message[BUF_SIZE];
    FILE *readfp;
    FILE *writefp;
    for (int i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, nullptr, nullptr);
        if (clnt_sock == -1)
            error_handling("accept() error");
        else {
            readfp = fdopen(clnt_sock, "r");
            writefp = fdopen(clnt_sock, "w");
            if (readfp == nullptr || writefp == nullptr)
                error_handling("fdopen() error");
            std::cout << "Connected client " << i + 1 << '\n';
        }
        while (!feof(readfp)) {
            fgets(message, BUF_SIZE - 1, readfp);
            std::cout << "Message from client: " << message;
            // 这里目的是：配合客户端fgets读取要求
            int len = strlen(message);
            message[len] = '\n';
            fputs(message, writefp);
            fflush(writefp);
        }
        fclose(readfp);
        fclose(writefp);
        std::cout << "Disconnect client " << i + 1 << '\n';
    }
    close(serv_sock);
    return 0;
}
