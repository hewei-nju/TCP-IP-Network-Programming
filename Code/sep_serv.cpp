/** @author heweibright@gmail.com 
 *  @date 2021/9/5 10:43
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
    writefp = fdopen(clnt_sock, "w");

    fputs("FROM SERVER: Hi~ client?\n", writefp);
    fputs("I love all of the world.\n", writefp);
    fputs("You are awesome!\n", writefp);
    fflush(writefp);

    /** 注意fclose(writefp)的调用效果 **/
    // 如果理解不深，便会认为这是半关闭
    fclose(writefp);
    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    fclose(readfp);
    return 0;
}
