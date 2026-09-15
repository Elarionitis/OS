#include <stdio.h>
#include <pthread.h>

long long counter = 0;

void *increment(void *arg)
{
    for (int i = 0; i < 500000; i++)
        counter++;

    return NULL;
}

int main()
{
    pthread_t threads[10];

    for (int i = 0; i < 10; i++)
        pthread_create(&threads[i], NULL, increment, NULL);

    for (int i = 0; i < 10; i++)
        pthread_join(threads[i], NULL);

    printf("Final counter = %lld\n", counter);

    return 0;
}
