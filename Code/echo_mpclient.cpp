/** @author heweibright@gmail.com 
 *  @date 2021/8/31 8:58
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 1024;


void read_routine(int sock, char *buf) {
    while (true) {
        int str_len = read(sock, buf, BUF_SIZE);
        if (str_len == 0)
            return;

        buf[str_len] = '\0';
        std::cout << "Message from server: " << buf << '\n';
    }
}

void write_routine(int sock, char *buf) {
    while (true) {
        std::cout << "Input message(Q to quit): ";
        if (std::cin.peek() == '\n')
            std::cin.get();
        std::cin.get(buf, BUF_SIZE, '\n');
        if (!strcmp(buf, "q\0") || !strcmp(buf, "Q\0")) {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));
        // Sleep for 5000 microseconds
        // Because in my computer, the son process always runs before the parent process. As such, there will be unexpected sequenced information printed on the bash/terminal.
        usleep(5000);
    }
}

void error_handling(const char *msg) {
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage : " << argv[0] << " <IP> <port\n";
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

    if (connect(sock, reinterpret_cast<sockaddr *>(&serv_adr), sizeof(serv_adr)) == -1)
        error_handling("connect() error");


    char buf[BUF_SIZE];
    pid_t pid = fork();
    if (pid == -1)
        error_handling("fork() error");

    if (pid == 0)
        write_routine(sock, buf);
    else
        read_routine(sock, buf);

    close(sock);
    return 0;
}
