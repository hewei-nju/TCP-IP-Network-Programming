/** @author heweibright@gmail.com 
 *  @date 2021/9/6 12:04
 *  Copyright (c) All rights reserved.
 */
 
#include <iostream>
#include <pthread.h>
#include <semaphore.h>

int num;
static sem_t sem_one;
static sem_t sem_two;

void * read(void *arg)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Input num: ";
        sem_wait(&sem_two);
        std::cin >> num;
        sem_post(&sem_one);
    }
    return nullptr;
}

void * accu(void *arg)
{
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_one);
        sum += num;
        sem_post(&sem_two);
    }
    std::cout << "result: " << sum << '\n';
    return nullptr;
}

int main(int argc, char *argv[])
{
    pthread_t tid1, tid2;
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);

    pthread_create(&tid1, nullptr, read, nullptr);
    pthread_create(&tid2, nullptr, accu, nullptr);

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}