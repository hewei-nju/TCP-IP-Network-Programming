/** @author heweibright@gmail.com 
 *  @date 2021/9/3 11:39
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <sys/uio.h>

const int BUF_SIZE = 1024;

int main(int argc, char *argv[])
{
    struct iovec vec[2];
    char buf1[BUF_SIZE];
    char buf2[BUF_SIZE];
    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = BUF_SIZE;

    ssize_t len;
    if ((len = readv(0, vec, 2)) == -1) {
        std::cerr << "readv() error\n";
        std::terminate();
    }
    std::cout << "Read bytes: " << len << '\n';
    std::cout << "First message: " << buf1 << '\n';
    std::cout << "Second message: " << buf2 << '\n';
    return 0;
}