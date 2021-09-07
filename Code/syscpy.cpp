/** @author heweibright@gmail.com 
 *  @date 2021/9/4 17:07
 *  Copyright (c) All rights reserved.
 */
 
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <chrono>

const int BUF_SIZE = 1024;

int main(int argc, char *argv[])
{
    int fd1, fd2;
    int len;
    char buf[BUF_SIZE];

    auto start = std::chrono::steady_clock::now().time_since_epoch().count();
    fd1 = open("news.txt", O_RDONLY);
    fd2 = open("cpy.txt", O_WRONLY | O_CREAT | O_TRUNC);
    while ((len = read(fd1, buf, sizeof(buf))) > 0)
        write(fd2, buf, len);

    close(fd1);
    close(fd2);
    auto end = std::chrono::steady_clock::now().time_since_epoch().count();
    std::cout << "Time: " << end - start << std::endl;
    return 0;
}
