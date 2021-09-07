/** @author heweibright@gmail.com 
 *  @date 2021/9/6 14:06
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

const int BUF_SIZE = 1024;
const int MAX_CLNT = 256;

void *handle_clnt(void *arg);

void *send_msg(const char *msg, int len);

void error_handling(const char *msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        std::terminate();
    }

    pthread_mutex_init(&mutex, nullptr);

    int serv_sock, clnt_sock;
    struct sockaddr_in addr;

    if ((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        error_handling("socket() error");
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    pthread_t tid;
    struct sockaddr_in clnt_addr;
    socklen_t addrlen;
    while (true) {
        if ((clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr *>(&clnt_addr), &addrlen)) == -1)
            error_handling("socket() error");

        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&tid, nullptr, handle_clnt, &clnt_sock);
        pthread_detach(tid);

        std::cout << "Connected client IP: " << inet_ntoa(clnt_addr.sin_addr) << '\n';
    }
    close(serv_sock);
    pthread_mutex_destroy(&mutex);
    return 0;
}

void *handle_clnt(void *arg) {
    int clnt_sock = *((int *) arg);
    char msg[BUF_SIZE];
    ssize_t len;
    while ((len = read(clnt_sock, msg, sizeof(msg))) != 0)
        send_msg(msg, len);

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clnt_cnt; ++i) {
        if (clnt_sock == clnt_socks[i]) {
            while (i++ < clnt_cnt - 1)
                clnt_socks[i] = clnt_socks[i + 1];
            break;
        }
    }
    clnt_cnt -= 1;
    pthread_mutex_unlock(&mutex);

    close(clnt_sock);
    std::cout << "Disconnected a client.\n";
    return nullptr;
}

void *send_msg(const char *msg, int len) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clnt_cnt; ++i) {
        if (write(clnt_socks[i], msg, len) == -1)
            error_handling("write() error");
    }
    pthread_mutex_unlock(&mutex);
}

void error_handling(const char *msg) {
    std::cerr << msg << '\n';
    std::terminate();
}