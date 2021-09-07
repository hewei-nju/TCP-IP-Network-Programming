/** @author heweibright@gmail.com 
 *  @date 2021/9/4 17:11
 *  Copyright (c) All rights reserved.
 */

#include <cstdio>
#include <unistd.h>
#include <iostream>

#include <chrono>

const int BUF_SIZE = 1024;

int main(int argc, char *argv[]) {
    FILE *fp1;
    FILE *fp2;
    char buf[BUF_SIZE];

    auto start = std::chrono::steady_clock::now().time_since_epoch().count();
    fp1 = fopen("news.txt", "r");
    fp2 = fopen("cpy.txt", "w");

    while (fgets(buf, sizeof(buf), fp1) != NULL)
        fputs(buf, fp2);
    fclose(fp1);
    fclose(fp2);

    auto end = std::chrono::steady_clock::now().time_since_epoch().count();
    std::cout << "Time: " << end - start << std::endl;
    return 0;
}
