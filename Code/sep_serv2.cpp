/** @author heweibright@gmail.com 
 *  @date 2021/9/5 11:42
 *  Copyright (c) All rights reserved.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

/** 注意：为了凸显问题，已经将所有的异常处理代码省略！大部分代码也是参考书上！ **/
const int BUF_SIZE = 1024;

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    FILE *readfp;
    FILE *writefp;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    char buf[BUF_SIZE] = {0,};

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr));
    listen(serv_sock, 5);
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_adr, &clnt_adr_sz);

    readfp = fdopen(clnt_sock, "r");
    // 使用dup复制出一个文件描述符副本
    writefp = fdopen(dup(clnt_sock), "w");

    fputs("FROM SERVER: Hi~ client?\n", writefp);
    fputs("I love all of the world.\n", writefp);
    fputs("You are awesome!\n", writefp);
    fflush(writefp);

    // 进行半关闭
    // 注意：调用shutdown会对该文件或套接字所有的文件描述符进行半关闭
    // 另外：不能调用shutdown(fileno(readfp), SHUT_WR)，没有效果！(我做过测试了！)
    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);

    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);

    /** 目的：测试对writefp进行shutdown后，是否能够通过readfp构造出可写的FILE指针
     *  事实：不能！但是writefp指针并不为空！但是不能写成功！
    writefp = fdopen(clnt_sock, "w");
    if (writefp == NULL)
        printf("NULL\n");
    fputs("after shutdown\n", writefp);
    */

    fclose(readfp);
    return 0;
}