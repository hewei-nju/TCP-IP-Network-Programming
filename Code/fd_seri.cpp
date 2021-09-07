/** @author heweibright@gmail.com 
 *  @date 2021/8/28 11:24
 *  Copyright (c) All rights reserved.
 */
 

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main()
{
    int fd1, fd2, fd3;
    fd1 = socket(PF_INET, SOCK_STREAM, 0);
    fd2 = open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
    fd3 = socket(PF_INET, SOCK_DGRAM, 0);

    std::cout << "file descriptor 1: " << fd1 << '\n';
    std::cout << "file descriptor 2: " << fd2 << '\n';
    std::cout << "file descriptor 3: " << fd3 << '\n';

    close(fd1);
    close(fd2);
    close(fd3);
    return 0;
}