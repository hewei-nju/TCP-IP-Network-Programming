/** @author heweibright@gmail.com 
 *  @date 2021/8/28 11:20
 *  Copyright (c) All rights reserved.
 */


#include <iostream>
#include <unistd.h>
#include <fcntl.h>

const int BUF_SIZE = 100;

void error_handling(const char* msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main()
{
    int fd;
    char buf[BUF_SIZE];
    fd = open("data.txt", O_RDONLY);
    if (fd == -1)
        error_handling("open() error!");
    std::cout << "file descriptor: " << fd << '\n';

    if (read(fd, buf, sizeof(buf)) == -1)
        error_handling("read() error!");
    std::cout << "file data: " << buf << '\n';
    close(fd);
    return 0;
}