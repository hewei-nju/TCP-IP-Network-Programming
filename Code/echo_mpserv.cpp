/** @author heweibright@gmail.com 
 *  @date 2021/8/30 23:43
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>

const int BUF_SIZE = 1024;

void error_handling(const char *msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

void read_childproc(int sig)
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    std::cout << "removed proc id: " << pid << '\n';
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        std::terminate();
    }

    // 注册子进程终止信息
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr;

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    while (true) {
        if ((clnt_sock = accept(serv_sock, 0, 0)) == -1)
            continue;
        else
            std::cout << "connected client: " << clnt_sock << '\n';

        pid_t pid = fork();
        if (pid == -1) {
            close(clnt_sock);
            continue;
        }

        char message[BUF_SIZE];
        ssize_t len;
        if (pid == 0) {
            // 因为fork成功会复制父进程所有资源，包括套接字文件描述符
            // 但是子进程使用不到serv_sock，而且套接字被销毁前，套接字的文件描述符必须都被销毁
            close(serv_sock);

            while ((len = read(clnt_sock, message, BUF_SIZE - 1)) != -1 && len != 0) {
                message[len] = '\0';
                if (write(clnt_sock, message, len) == -1)
                    error_handling("write() error");
            }
            close(clnt_sock);
            if (len == -1)
                error_handling("read() error");
            std::cout << "disconnect client: " << clnt_sock << '\n';
            return 0;
        } else {
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}

