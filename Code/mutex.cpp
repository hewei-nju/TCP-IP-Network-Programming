/** @author heweibright@gmail.com 
 *  @date 2021/9/6 11:40
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <pthread.h>

const int MAX_THREAD = 100;
long long num = 0;
pthread_mutex_t mutex;

void * thread_inc(void *arg)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 50000000; ++i)
        num += 1;
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

void * thread_des(void *arg)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 50000000; ++i)
        num -= 1;
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

int main(int argc, char *argv[])
{
    pthread_t thread_id[MAX_THREAD];
    std::cout << "sizeof(long long): " << sizeof(long long) << '\n';

    pthread_mutex_init(&mutex, nullptr);

    for (int i = 0; i < MAX_THREAD; ++i) {
        if (i & 1)
            pthread_create(&thread_id[i], nullptr, thread_inc, nullptr);
        else
            pthread_create(&thread_id[i], nullptr, thread_des, nullptr);
    }

    for (int i = 0; i < MAX_THREAD; ++i)
        pthread_join(thread_id[i], nullptr);

    pthread_mutex_destroy(&mutex);

    std::cout << "result: " << num << '\n';
    return 0;
}

