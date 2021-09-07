/** @author heweibright@gmail.com 
 *  @date 2021/9/6 8:56
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <unistd.h>
#include <pthread.h>

void *thread_main(void *arg)
{
    int cnt = *((int *)arg);
    for (int i = 0; i < cnt; ++i) {
        sleep(1);
        std::cout << "running thread\n";
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    pthread_t t_id;
    int thread_param = 5;
    if (pthread_create(&t_id, nullptr, thread_main, &thread_param) != 0) {
        std::cout << "pthread_create() error\n";
        std::terminate();
    }
    sleep(10);
    std::cout << "end of main\n";
    return 0;
}
