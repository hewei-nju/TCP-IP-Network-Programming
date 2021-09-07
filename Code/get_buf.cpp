/** @author heweibright@gmail.com 
 *  @date 2021/8/29 20:24
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(const char *msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[])
{
    int sock;
    int snd_buf, rcv_buf;
    socklen_t optlen = sizeof(snd_buf);
    int state;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, &optlen);
    if (state == -1)
        error_handling("getsockopt() error");
    std::cout << "Output buffer size: " << snd_buf << '\n';

    optlen = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, &optlen);
    if (state == -1)
        error_handling("getsockopt() error");
    std::cout << "Input buffer size: " << rcv_buf << '\n';

    return 0;

}

