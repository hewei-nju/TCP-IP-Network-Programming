/** @author heweibright@gmail.com 
 *  @date 2021/8/30 0:45
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        sleep(15);
        return 24;
    } else {
        while (!waitpid(-1, &status, WNOHANG)) {
            sleep(3);
            std::cout << "sleep 3sec.\n";
        }

        if (WIFEXITED(status))
            std::cout << "Child send " << WEXITSTATUS(status) << '\n';
    }
    return 0;
}