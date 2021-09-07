/** @author heweibright@gmail.com 
 *  @date 2021/8/29 20:55
 *  Copyright (c) All rights reserved.
 */
 

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(const char *msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main()
{
    int serv_sock, clnt_sock;
    int option, str_len;


}