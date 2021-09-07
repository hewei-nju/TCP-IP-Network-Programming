/** @author heweibright@gmail.com 
 *  @date 2021/9/2 6:09
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

const int BUF_SIZE = 1024;

int main(int argc, char *argv[])
{
    fd_set reads, temps;
    FD_ZERO(&reads);
    FD_SET(0, &reads);  // 0 is standard input(console)

    int result, len;
    char buf[BUF_SIZE];
    struct timeval timeout;
    while (true) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        result = select(1, &temps, 0, 0, &timeout);
        if (result == -1) {
            std::cerr << "select() error\n";
            std::terminate();
        } else if (result == 0) {
            std::cout << "Time-out!\n";
        } else {
            if (FD_ISSET(0, &temps)) {
                len = read(0, buf, BUF_SIZE);
                if (len == -1) {
                    std::cerr << "read() error\n";
                    std::terminate();
                }
                buf[len] = '\0';
                std::cout << "Message from standard input: " << buf;
            }
        }
    }
    return 0;
}