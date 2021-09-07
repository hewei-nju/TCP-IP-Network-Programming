/** @author heweibright@gmail.com 
 *  @date 2021/8/28 14:30
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    const char* addr1 = "1.2.3.4";
    const char* addr2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(addr1);
    if (conv_addr == INADDR_NONE)
        std::cerr << "Error occured!\n";
    else
        std::cout << "Network ordered integer addr: " << std::hex << conv_addr << '\n';

    conv_addr = inet_addr(addr2);
    if (conv_addr == INADDR_NONE)
        std::cerr << "Error occured!\n";
    else
        std::cout << "Network ordered integer addr: " << std::hex << conv_addr << '\n';
    return 0;
}
