/** @author heweibright@gmail.com 
 *  @date 2021/8/29 20:11
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
    int tcp_sock, udp_sock;
    int sock_type;
    socklen_t optlen = sizeof(sock_type);
    int state;

    tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (tcp_sock == -1 || udp_sock == -1)
        error_handling("socket() error");
    std::cout << "SOCK_STREAM: " << SOCK_STREAM << '\n';
    std::cout << "SOCK_DGRAM: " << SOCK_DGRAM << '\n';

    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, &sock_type, &optlen);
    if (state == -1)
        error_handling("getsockopt() error");
    std::cout << "tcp socket type: " << sock_type << '\n';

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, &sock_type, &optlen);
    if (state == -1)
        error_handling("getsocket() error");
    std::cout << "udp socket type: " << sock_type << '\n';

    return 0;

}
