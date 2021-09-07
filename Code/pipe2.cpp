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
    int fds[2];
    char str1[] = "Who are you?";
    char str2[] = "I am bright. Thank you for your message!";
    char buf[BUF_SIZE];

    if (pipe(fds) == -1)
        error_handling("pipe() error");

    pid_t pid = fork();
    if (pid == -1)
        error_handling("fork() error");

    if (pid == 0) {
        if (write(fds[1], str1, sizeof(str1)) == -1)
            error_handling("write() error");
        std::cout << "Child process write message: " << str1 << '\n';
        // Make the child process sleep for 50 microseconds. Aims to let the parent process passes the data correctly.
        usleep(50);
        if (read(fds[0], buf, BUF_SIZE) == -1)
            error_handling("read() error");
        std::cout << "Message from parent process: " << buf << '\n';
    } else {
        if (read(fds[0], buf, BUF_SIZE) == -1)
            error_handling("read() error");
        std::cout << "Message from child process: " << buf << '\n';
        if (write(fds[1], str2, sizeof(str2)) == -1)
            error_handling("write() error");
        std::cout << "Parent process write message: " << str2 << '\n';
        // To prevent the bash/terminal presents the command prompt.
        // Of course, in the if branch, I just use "usleep(50)" for the child process, so it's too short to present the command prompt. Using "sleep(2)" and then you will see the result.
        usleep(50);
    }
    return 0;
}