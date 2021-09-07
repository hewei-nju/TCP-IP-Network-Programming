/** @author heweibright@gmail.com 
 *  @date 2021/8/30 23:26
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        std::cout << "Removed proc id: " << id << '\n';
        std::cout << "Child send: " << WEXITSTATUS(status) << '\n';
    }
}

int main(int argc, char *argv[])
{
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    pid_t pid = fork();
    if (pid == 0) {
        std::cout << "Hi! I'm child process\n";
        sleep(10);
        return 12;
    } else {
        std::cout << "Child proc id: " << pid << '\n';
        pid = fork();
        if (pid == 0) {
            std::cout << "Hi! I'm child process\n";
            sleep(10);
            return 20;
        } else {
            std::cout << "Child proc id: " << pid << '\n';
            for (int i = 0; i < 5; ++i) {
                std::cout << "wait...\n";
                sleep(5);
            }
        }
    }
    return 0;
}
