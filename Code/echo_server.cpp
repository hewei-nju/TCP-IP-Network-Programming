/** @author heweibright@gmail.com 
 *  @date 2021/8/28 20:29
 *  Copyright (c) All rights reserved.
 */
 

#include <iostream>
#include <cstring>
#include <cstdlib>
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
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];

    sockaddr_in serv_addr;
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        std::terminate();
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

//    int option;
//    socklen_t optlen = sizeof(option);
//    option = 1;
//    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen);


    if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    for (int i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, 0, 0);
        if (clnt_sock == -1)
            error_handling("accept() error");
        else
            std::cout << "Connected client " << i + 1 << '\n';
        int str_len;
        while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
            write(clnt_sock, message, str_len);
        }
        close(clnt_sock);
    }
    close(serv_sock);

    return 0;
}