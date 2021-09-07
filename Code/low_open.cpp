/** @author heweibright@gmail.com 
 *  @date 2021/8/28 11:14
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

void error_handling(const char* msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main()
{
    int fd;
    char buf[] = "Let's go!\n";
    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
    if (fd == -1)
        error_handling("open() error!");
    std::cout << "file descriptor: " << fd << '\n';

    if (write(fd, buf, sizeof(buf)) == -1)
        error_handling("write() error!");
    close(fd);
    return 0;
}