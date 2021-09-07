/** @author heweibright@gmail.com 
 *  @date 2021/9/4 19:36
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
    fp = fdopen(fd, "w");
    fputs("Network C programming\n", fp);
    fclose(fp);
    return 0;
}