/** @author heweibright@gmail.com 
 *  @date 2021/9/6 14:54
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <pthread.h>

const int BUF_SIZE = 1024;
const int NAME_SIZE = 20;

void *send_msg(void *arg);

void *recv_msg(void *arg);

void error_handling(const char *msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <IP> <port> <name>\n";
        std::terminate();
    }

    int sock;
    struct sockaddr_in serv_addr;

    sprintf(name, "[%s]", argv[3]);
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    pthread_t snd_thread, rcv_thread;
    void *thread_return;
    pthread_create(&snd_thread, nullptr, send_msg, &sock);
    pthread_create(&rcv_thread, nullptr, recv_msg, &sock);
    pthread_join(snd_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);

    close(sock);
    return 0;
}

void *send_msg(void *arg) {
    int sock = *((int *) arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    while (true) {
        if (std::cin.peek() == '\n')
            std::cin.get();
        std::cin.get(msg, BUF_SIZE, '\n');
        if (!(strcmp(msg, "q\0") != 0 && strcmp(msg, "Q\0") != 0)) {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return nullptr;
}

void *recv_msg(void *arg) {
    int sock = *((int *) arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    int str_len;
    while (true) {
        str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);
        if (str_len == -1)
            return reinterpret_cast<void *>(-1);
        name_msg[str_len] = '\0';
        std::cout << name_msg << '\n';
    }
    return nullptr;
}

void error_handling(const char *msg) {
    std::cerr << msg << '\n';
    std::terminate();
}
