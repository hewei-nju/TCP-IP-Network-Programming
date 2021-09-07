/** @author heweibright@gmail.com 
 *  @date 2021/9/5 15:59
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

const int BUF_SIZE = 1024;
const int EPOLL_SIZE = 64;

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
    struct sockaddr_in serv_addr;

    if ((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    int epfd;
    int event_cnt;
    char message[BUF_SIZE];
    struct epoll_event *ep_events;
    struct epoll_event event;
    if ((epfd = epoll_create(EPOLL_SIZE)) == -1)
        error_handling("epoll_create() error");
    ep_events = new struct epoll_event[EPOLL_SIZE];
    event.events = EPOLLIN;
    event.data.fd = serv_sock;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event) == -1)
        error_handling("epoll_ctl() error");

    while (true) {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1) {
            error_handling("epoll_wait() error");
            break;
        }
        for (int i = 0; i < event_cnt; ++i) {
            if (ep_events[i].data.fd == serv_sock) {
                if ((clnt_sock = accept(serv_sock, 0, 0)) == -1)
                    error_handling("accept() error");
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                std::cout << "Connected client: " << clnt_sock << '\n';

            } else {
                ssize_t len = read(ep_events[i].data.fd, message, BUF_SIZE);
                if (len == -1)
                    error_handling("read() error");
                else if (len == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    std::cout << "Closed client: " << ep_events[i].data.fd << '\n';
                } else {
                    if (write(ep_events[i].data.fd, message, len) == -1)
                        error_handling("write() error");
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}