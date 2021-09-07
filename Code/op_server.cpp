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

int add(int a, int b)
{
    return a + b;
}

int minus(int a, int b)
{
    return a - b;
}

int multiple(int a, int b)
{
    return a * b;
}

int divide(int a, int b)
{
    if (b == 0)
        error_handling("divisor is 0");
    return a / b;
}

int calculate(int n, int(*func)(int, int), int src[])
{
    if (n == 1)
        return *src;
    for (int i = 1; i < n; ++i)
        *src = func(*src, src[i]);
    return *src;
}


int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: <IP> <port>\n";
        std::terminate();
    }

    int serv_sock, clnt_sock;
    sockaddr_in serv_addr;

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    for (int i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, 0, 0);
        if (clnt_sock == -1)
            error_handling("accept() error");
        else
            std::cout << "Connected client " << i + 1 << '\n';

        int opCnt;
        read(clnt_sock, &opCnt, sizeof(opCnt));
        std::cout << "Operand count: " << opCnt << '\n';

        int arr[opCnt];
        read(clnt_sock, arr, sizeof(arr));
        for (int i = 0; i < opCnt; ++i)
            std::cout << "Operand " << i + 1 << ": " << arr[i] << '\n';

        int result;
        char opcode;
        read(clnt_sock, &opcode, sizeof(opcode));
        std::cout << "Operator: " << opcode << '\n';
        switch (opcode) {
            case '+': result = calculate(opCnt, add, arr); break;
            case '-': result = calculate(opCnt, minus, arr); break;
            case '*': result = calculate(opCnt, multiple, arr); break;
            case '/': result = calculate(opCnt, divide, arr); break;
        }
        std::cout << "Operation result: " << result << "\n\n";
        write(clnt_sock, &result, sizeof(result));
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}