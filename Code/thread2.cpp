/** @author heweibright@gmail.com 
 *  @date 2021/9/6 9:15
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

void *thread_main(void *arg)
{
    int cnt = *((int *)arg);
    char *msg = (char *)malloc(sizeof(char)*50);
    strcpy(msg, "Hello, I am thread~\n");
    for (int i = 0; i < cnt; ++i) {
        sleep(1);
        std::cout << "running thread\n";
    }
    return msg;
}

int main(int argc, char *argv[])
{
    pthread_t t_id;
    int thread_param = 5;
    void *thr_ret;

    if (pthread_create(&t_id, nullptr, thread_main, &thread_param) != 0) {
        std::cout << "pthread_create() error\n";
        std::terminate();
    }

    if (pthread_join(t_id, &thr_ret) != 0) {
        std::cout << "pthread_join() error\n";
        std::terminate();
    }

    std::cout << "Thread return message: " << static_cast<char *>(thr_ret);
    free(thr_ret);
    return 0;
}