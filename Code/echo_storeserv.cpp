#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>

const int BUF_SIZE = 1024;

void error_handling(const char *msg) {
    std::cerr << msg << '\n';
    std::terminate();
}

void read_childproc(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    std::cout << "removed proc id: " << pid << '\n';
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        std::terminate();
    }

    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, reinterpret_cast<sockaddr *>(&serv_adr), sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    int fds[2];
    int str_len;
    char buf[BUF_SIZE];

    if (pipe(fds) == -1)
        error_handling("pipe() error");

    pid_t pid = fork();
    if (pid == -1)
        error_handling("fork() error");
    // 子进程1：用来保存客户端发来的消息
    if (pid == 0) {
        std::ofstream file;
        file.open("echomsg.txt", std::fstream::out | std::fstream::app);
        if (!file.is_open())
            error_handling("open() error");

        char msgbuf[BUF_SIZE];
        int len;

        // 只能读取前10条内容并保存
        // 只是作为一个实例，当然，如果想要实现为无数条，并且不受进程退出影响，程序会麻烦很多
        // 已实现实时存储无数条echo消息的功能
        while ((len = read(fds[0], msgbuf, BUF_SIZE)) != -1 && len != 0) {
            msgbuf[len] = '\0';
            std::cout << msgbuf << '\n';
            if (file.write(msgbuf, len)) {
                file << '\n';
                file.flush();
                std::cout << "write successfully\n";
            }
        }
        if (len == -1)
            error_handling("read() error");
        file.close();
        return 0;
    }

    // 进行回显客户端消息的主进程
    while (true) {
        clnt_sock = accept(serv_sock, 0, 0);
        if (clnt_sock == -1)
            continue;
        else
            std::cout << "Connected client: " << clnt_sock << '\n';

        pid = fork();
        if (pid == -1)
            error_handling("fork() error");
        // 子进程2：用来将客户端发来的消息传回去
        if (pid == 0) {
            close(serv_sock);
            while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != -1 && str_len != 0) {
                if (write(clnt_sock, buf, str_len) == -1)
                    error_handling("write() error");
                if (write(fds[1], buf, str_len) == -1)
                    error_handling("write() error");
            }
            close(clnt_sock);
            std::cout << "Client " << clnt_sock << " disconnected\n";
            return 0;
        } else
            close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}