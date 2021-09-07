/** @author heweibright@gmail.com 
 *  @date 2021/8/29 15:50
 *  Copyright (c) All rights reserved.
 */
 

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(const char *msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <addr>\n";
        std::terminate();
    }

    hostent *host = gethostbyname(argv[1]);
    if (host == NULL)
        error_handling("gethostbyname() error");

    std::cout << "Official name: " << host->h_name << '\n';
    for (int i = 0; host->h_aliases[i]; ++i) {
        std::cout << "Aliases " << i + 1 << ": " << host->h_aliases[i] << '\n';
    }
    std::cout << "Address type: " << (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6") << '\n';
    for (int i = 0; host->h_aliases[i]; ++i) {
        std::cout << "IP address: " << i + 1 << inet_ntoa(*reinterpret_cast<in_addr *>(host->h_addr_list[i])) << '\n';
    }
    return 0;
}