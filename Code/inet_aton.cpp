/** @author heweibright@gmail.com 
 *  @date 2021/8/28 14:39
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <arpa/inet.h>

void error_handling(const char* msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}


int main(int argc, char *argv[])
{
    const char* addr = "127.232.124.79";
    sockaddr_in addr_inet;

    if (!inet_aton(addr, &addr_inet.sin_addr))
        error_handling("Conversion error");
    else
        std::cout << "Network ordered integer addr: " << addr_inet.sin_addr.s_addr << '\n';
    return 0;
}

