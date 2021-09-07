/** @author heweibright@gmail.com 
 *  @date 2021/8/29 9:19
 *  Copyright (c) All rights reserved.
 */
 

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>

const int PATH_SIZE = 64;
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

    int serv_sock, clnt_sock;
    sockaddr_in serv_addr;

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    if ((clnt_sock = accept(serv_sock, 0, 0)) == -1)
        error_handling("accept() error");

    char filepath[PATH_SIZE];
    ssize_t len = read(clnt_sock, filepath, PATH_SIZE);
    if (len == -1)
        error_handling("read() error");
    filepath[len] = '\0';

    std::ifstream file(filepath, std::fstream::in);
    if (!file.is_open())
        error_handling("file open() error");

    char content[BUF_SIZE];
    while (file.read(content, BUF_SIZE - 1)) {
        len = file.gcount();
        write(clnt_sock, content, len);
    }
    len = file.gcount();
    write(clnt_sock, content, len);
    file.close();

    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, content, BUF_SIZE);
    std::cout << "Message from client: \n" << content << '\n';

    close(clnt_sock);
    close(serv_sock);
    return 0;
}