/** @author heweibright@gmail.com 
 *  @date 2021/9/5 10:52
 *  Copyright (c) All rights reserved.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

const int BUF_SIZE = 1024;

int main(int argc, char *argv[]) {
    int sock;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_addr;

    FILE *readfp;
    FILE *writefp;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w");

    while (true) {
        if (fgets(buf, sizeof(buf), readfp) == NULL)
            break;
        fputs(buf, stdout);
        fflush(stdout);
    }

    fputs("FROM CLIENT: Thank you!\n", writefp);
    fflush(writefp);

    /** 目的：测试对sep_serv2.cpp中对writefp进行shutdown后，是否能够通过readfp构造出可写的FILE指针
     *  结果：事实证明，对一个文件描述符进行版关闭后，该套接字的所有这个操作(读或写)都将被关闭！
    memset(buf, 0, sizeof(buf));
    if (fgets(buf, sizeof(buf), readfp) != NULL)
        fputs(buf, stdout);
    fflush(stdout);
    */

    fclose(writefp);
    fclose(readfp);
    return 0;
}

