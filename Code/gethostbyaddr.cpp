/** @author heweibright@gmail.com 
 *  @date 2021/8/29 16:17
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

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    hostent *host = gethostbyaddr(reinterpret_cast<char *>(&addr.sin_addr), 4, AF_INET);
    if (host == NULL)
        error_handling("gethostbyaddr() error");
    else {
        switch (h_errno) {
            case HOST_NOT_FOUND:
                std::cerr << "HOST_NOT_FOUND\n";
                break;
            case NO_ADDRESS:
                std::cerr << "NO_ADDRESS or NO_DATA\n";
                break;
            case NO_RECOVERY:
                std::cerr << "NO_RECOVERY\n";
                break;
            case TRY_AGAIN:
                std::cerr << "TRY_AGAIN\n";
        }
    }

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
