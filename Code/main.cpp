/** @author heweibright@gmail.com
 *  @date 2021/8/29 10:56
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;


int main()
{
    ifstream file;
    file.open("/home/bright/C++/news.txt", std::fstream::in);
    char msg[10];
    if (!file.is_open())
        return -1;
    while (file.read(msg, 9)) {
        msg[9] = '\0';
        cout << msg;
        memset(msg, 0, sizeof(msg));
    }
    cout << msg;
}