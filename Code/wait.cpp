/** @author heweibright@gmail.com 
 *  @date 2021/8/29 23:56
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status;
    pid_t pid = fork();

    if (pid == 0)
        return 3;
    else {
        std::cout << "Child PID: " << pid << '\n';

        pid = fork();
        if (pid == 0)
            exit(7);
        else {
            std::cout << "Child PID: " << pid << '\n';
            wait(&status);
            if (WIFEXITED(status))
                std::cout << "Child send one: " << WEXITSTATUS(status) << '\n';

            wait(&status);
            if (WIFEXITED(status))
                std::cout << "Child send two: " << WEXITSTATUS(status) << '\n';
            sleep(30);
        }
    }
}
