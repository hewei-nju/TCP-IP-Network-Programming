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
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <port>\n";
        std::terminate();
    }

    int clnt_sock;
    sockaddr_in serv_addr;

    clnt_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (clnt_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(clnt_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    char src_path[PATH_SIZE], dest_path[PATH_SIZE];
    std::cout << "source filepath: ";
    std::cin >> src_path;
    std::cout << "destination filepath: ";
    std::cin >> dest_path;
    write(clnt_sock, src_path, strlen(src_path));

    ssize_t len;
    char content[BUF_SIZE];
    std::ofstream file;
    file.open(dest_path, std::fstream::out | std::fstream::trunc);
    if (!file.is_open())
        error_handling("file open() error");
    std::cout << "file transfer...";
    while ((len = read(clnt_sock, content, BUF_SIZE)) != 0) {  // read直到接收EOF
        std::cout << "...";
        file.write(content, len);
    }
    if (len != -1)
        file.write(content, len);
    file.close();
    std::cout << '\n';
    std::cout << "Received file data\n";
    write(clnt_sock, "Thank you", 10);
    close(clnt_sock);

    return 0;
}