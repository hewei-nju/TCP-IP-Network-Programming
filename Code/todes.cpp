/** @author heweibright@gmail.com 
 *  @date 2021/9/4 19:43
 *  Copyright (c) All rights reserved.
 */
 
#include <cstdio>
#include <fcntl.h>

int main(void)
{
    FILE *fp;
    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) {
        fputs("file open error", stdout);
        return -1;
    }
    printf("First file descriptor: %d\n", fd);
    fp = fdopen(fd, "w");
    fputs("TCP/IP SOCKET PROGRAMMING\n", fp);
    printf("Second file descriptor: %d\n", fileno(fp));
    fclose(fp);
    return 0;
}
