/** @author heweibright@gmail.com 
 *  @date 2021/8/29 23:30
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid == 0) {
        std::cout << "Hi, I am a child process\n";
    } else {
        std::cout << "Child Process ID: " << pid << '\n';
        sleep(30);
    }

    if (pid == 0)
        std::cout << "End child process\n";
    else
        std::cout << "End parent process\n";
    return 0;
}

