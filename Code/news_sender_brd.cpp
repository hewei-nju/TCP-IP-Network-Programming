/** @author heweibright@gmail.com 
 *  @date 2021/9/4 16:48
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

const int TTL = 64;
const int BUF_SIZE = 1024;

void error_handling(const char *msg) {
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <GroupIP> <port>\n";
        std::terminate();
    }

    int sock;
    struct sockaddr_in addr;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
        error_handling("socket() error");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int so_brd = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &so_brd, sizeof(so_brd));

    std::ifstream file;
    char buf[BUF_SIZE];
    file.open("/home/bright/C++/news.txt", std::fstream::in);
    if (!file.is_open())
        error_handling("open() error");
    std::cout << "\033[33;40mStart to send news.\033[0m\n";
    std::cout << "\033[32;40m[  0%]";
    while (file.read(buf, sizeof(buf) - 1)) {
        sendto(sock, buf, strlen(buf), 0, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
        std::cout << "====" << std::flush;
        memset(buf, 0, sizeof(buf));
        sleep(2);
    }
    sendto(sock, buf, strlen(buf), 0, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
    std::cout << "====" << std::flush;
    std::cout << ">[100%]\033[0m\n";
    std::cout << "\033[33;40mSuccessful sending.\033[0m\n";
    file.close();
    close(sock);
}

