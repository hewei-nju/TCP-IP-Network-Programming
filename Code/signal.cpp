/** @author heweibright@gmail.com 
 *  @date 2021/8/30 8:50
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if (sig == SIGALRM)
        std::cout << "Time out!\n";
    alarm(2);
}

void keycontrol(int sig)
{
    if (sig == SIGINT)
        std::cout << "CTRL + C pressed\n";
//    std::terminate();
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for (int i = 0; i < 3; ++i) {
        std::cout << "wait...\n";
        sleep(100);
    }
}