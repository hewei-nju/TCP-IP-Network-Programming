/** @author heweibright@gmail.com 
 *  @date 2021/8/28 12:56
 *  Copyright (c) All rights reserved.
 */


#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void error_handling(const char *msg) {
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];

    if (argc != 3) {
        std::cout << "Usage : " << argv[0] << " <IP> <port>\n";
        return 1;
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) == -1)
        error_handling("connect() error!");


    int read_len = 0, index = 0, cnt = 0;
    while ((read_len = (read_len = read(sock, &message[index++], 1)))) {
        if (read_len == -1)
            error_handling("read() error!");
        cnt += 1;
    }
    std::cout << "Message from server: " << message << '\n';
    std::cout << "Function read call count: " << cnt << '\n';
    close(sock);
    return 0;
}
