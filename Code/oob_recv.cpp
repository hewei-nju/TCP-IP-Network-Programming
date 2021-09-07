/** @author heweibright@gmail.com 
 *  @date 2021/9/2 17:03
 *  Copyright (c) All rights reserved.
 */

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

const int BUF_SIZE = 1024;

int acpt_sock;
int recv_sock;

void urg_handler(int signo) {
    char buf[BUF_SIZE];
    ssize_t str_len = recv(recv_sock, buf, sizeof(buf), MSG_OOB);
    buf[str_len] = '\0';
    std::cout << "Urgent message: " <<  buf << '\n';
}

void error_handling(const char *msg) {
    std::cerr << msg << '\n';
    std::terminate();
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        std::terminate();
    }

    struct sigaction act;
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    struct sockaddr_in recv_adr;

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(acpt_sock, (struct sockaddr *) &recv_adr, sizeof(recv_adr)) == -1)
        error_handling("bind() error");
    if (listen(acpt_sock, 5) == -1)
        error_handling("listen() error");

    if ((recv_sock = accept(acpt_sock, 0, 0)) == -1)
        error_handling("accept() error");

    fcntl(recv_sock, F_SETOWN, getpid());
    sigaction(SIGURG, &act, 0);

    ssize_t len;
    char buf[BUF_SIZE];
    while ((len = recv(recv_sock, buf, sizeof(buf), 0)) != 0) {
        if (len == -1)
            continue;
        buf[len] = '\0';
        std::cout << buf << '\n';
    }
    close(recv_sock);
    close(acpt_sock);
    return 0;
}
