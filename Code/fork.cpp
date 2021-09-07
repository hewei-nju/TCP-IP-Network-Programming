/** @author heweibright@gmail.com 
 *  @date 2021/8/29 23:05
 *  Copyright (c) All rights reserved.
 */


#include <iostream>
#include <unistd.h>

int gval = 10;

int main(int argc, char *argv[]) {
    pid_t pid;
    int lval = 20;
    gval += 1;
    lval += 5;

    pid = fork();

    if (pid == 0) {
        gval += 2;
        lval += 2;
    } else {
        gval -= 2;
        lval -= 2;
    }

    if (pid == 0) {
        std::cout << "Child Proc: [" << gval << ", " << lval << "]\n";
    } else {
        std::cout << "Parent Proc: [" << gval << ", " << lval << "]\n";
    };
}
