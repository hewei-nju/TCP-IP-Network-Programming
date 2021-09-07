#include <iostream>
#include <unistd.h>

const int BUF_SIZE = 1024;

void error_handling(const char *msg)
{
    std::cerr << msg << '\n';
    std::terminate();
}

int main(int argc, char *argv[])
{
    int fds1[2], fds2[2];
    char str1[] = "Who are you?";
    char str2[] = "I am bright. Thank you for your message!";
    char buf[BUF_SIZE];

    if (pipe(fds1) == -1 || pipe(fds2) == -1)
        error_handling("pipe() error");

    pid_t pid = fork();
    if (pid == -1)
        error_handling("fork() error");

    if (pid == 0) {
        if (write(fds1[1], str1, sizeof(str1)) == -1)
            error_handling("write() error");
        std::cout << "Child process write message: " << str1 << '\n';
        if (read(fds2[0], buf, BUF_SIZE) == -1)
            error_handling("read() error");
        std::cout << "Message from parent process: " << buf << '\n';
    } else {
        if (read(fds1[0], buf, BUF_SIZE) == -1)
            error_handling("read() error");
        std::cout << "Message from child process: " << buf << '\n';
        if (write(fds2[1], str2, sizeof(str2)) == -1)
            error_handling("write() error");
        std::cout << "Parent process write message: " << str2 << '\n';
    }
    return 0;
}