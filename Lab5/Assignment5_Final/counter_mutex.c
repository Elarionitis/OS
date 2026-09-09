#include <stdio.h>
#include <pthread.h>

long long counter = 0;
pthread_mutex_t lock;

void *increment(void *arg)
{
    for (int i = 0; i < 500000; i++)
    {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main()
{
    pthread_t threads[10];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < 10; i++)
        pthread_create(&threads[i], NULL, increment, NULL);

    for (int i = 0; i < 10; i++)
        pthread_join(threads[i], NULL);

    printf("Final counter = %lld\n", counter);

    pthread_mutex_destroy(&lock);

    return 0;
}
