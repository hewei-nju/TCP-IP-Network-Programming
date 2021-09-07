/** @author heweibright@gmail.com 
 *  @date 2021/9/3 11:39
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <sys/uio.h>

int main(int argc, char *argv[])
{
    struct iovec vec[2];
    char buf1[] = "ABCDEFG";
    char buf2[] = "123456789";
    vec[0].iov_base = buf1;
    vec[0].iov_len = 3;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;

    ssize_t len;
    if ((len = writev(1, vec, 2)) == -1) {
        std::cerr << "writev() error\n";
        std::terminate();
    }
    std::cout << "\nWrite bytes: " << len << '\n';
    return 0;
}
