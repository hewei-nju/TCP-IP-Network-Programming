/** @author heweibright@gmail.com 
 *  @date 2021/8/28 22:01
 *  Copyright (c) All rights reserved.
 */


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

const int BUF_SIZE = 1024;
void error_handling(const char* msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <port>\n";
        std::terminate();
    }

    int sock;
    sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    if (connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
        error_handling("connect() error");
    std::cout << "Connected........\n";

    int n;
    std::cout << "Operand count: ";
    std::cin >> n;
    write(sock, &n, sizeof(n));

    int arr[n];
    for (int i = 0; i < n; ++i) {
        std::cout << "Operand " << i + 1 << ": ";
        std::cin >> arr[i];
    }
    write(sock, arr, sizeof(arr));

    char op;
    std::cout << "Operator: ";
    std::cin >> op;
    write(sock, &op, sizeof(op));

    int result;
    read(sock, &result, sizeof(result));
    std::cout << "Operation result: " << result << "\n\n";
    close(sock);

    return 0;
}