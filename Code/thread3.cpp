/** @author heweibright@gmail.com 
 *  @date 2021/9/6 9:44
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>

int sum = 0;
void *thread_summation(void *arg)
{
    int start = ((int *)arg)[0];
    int end = ((int *)arg)[1];
    while (start <= end) {
        sum += start++;
        if (start <= 6)
            std::cout << "worker 1\n";
        else
            std::cout << "worker 2\n";
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    pthread_t tid1, tid2;
    int range1[] = {1, 5};
    int range2[] = {6, 10};

    pthread_create(&tid1, nullptr, thread_summation, &range1);
    pthread_create(&tid2, nullptr, thread_summation, &range2);

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);
    std::cout << "result: " << sum << '\n';

    return 0;
}