#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define SIZE 10
#define PRODUCERS 3
#define CONSUMERS 2
#define RUN_TIME 30

int queue[SIZE];
int front = 0;
int rear = 0;
int count = 0;
int running = 1;

pthread_mutex_t lock;
pthread_cond_t not_full;
pthread_cond_t not_empty;

void enqueue_job(int job)
{
    pthread_mutex_lock(&lock);

    while (count == SIZE && running)
        pthread_cond_wait(&not_full, &lock);

    if (!running)
    {
        pthread_mutex_unlock(&lock);
        return;
    }

    queue[rear] = job;
    rear = (rear + 1) % SIZE;
    count++;

    printf("Produced Job %d | Queue = %d\n", job, count);

    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&lock);
}

int dequeue_job()
{
    pthread_mutex_lock(&lock);

    while (count == 0 && running)
        pthread_cond_wait(&not_empty, &lock);

    if (count == 0 && !running)
    {
        pthread_mutex_unlock(&lock);
        return -1;
    }

    int job = queue[front];
    front = (front + 1) % SIZE;
    count--;

    printf("Consumer took Job %d | Queue = %d\n", job, count);

    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&lock);

    return job;
}

void *producer(void *arg)
{
    int id = *(int *)arg;

    while (running)
    {
        int job = rand() % 1000 + 1;

        printf("Producer %d creating Job %d\n", id, job);
        enqueue_job(job);

        sleep(1);
    }

    return NULL;
}

void *consumer(void *arg)
{
    int id = *(int *)arg;

    while (1)
    {
        int job = dequeue_job();

        if (job == -1)
            break;

        printf("Consumer %d printing Job %d\n", id, job);
        sleep(2);
    }

    return NULL;
}

void *monitor_queue(void *arg)
{
    while (running)
    {
        sleep(2);

        pthread_mutex_lock(&lock);

        printf("\n--- Queue Status ---\n");
        printf("Jobs in queue = %d\n", count);

        if (count == 0)
            printf("Queue is EMPTY\n");
        else if (count == SIZE)
            printf("Queue is FULL\n");
        else
            printf("Queue has space\n");

        printf("--------------------\n\n");

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main()
{
    pthread_t producers[PRODUCERS];
    pthread_t consumers[CONSUMERS];
    pthread_t monitor;

    int producer_id[PRODUCERS] = {1, 2, 3};
    int consumer_id[CONSUMERS] = {1, 2};

    srand(time(NULL));

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    for (int i = 0; i < PRODUCERS; i++)
        pthread_create(&producers[i], NULL, producer, &producer_id[i]);

    for (int i = 0; i < CONSUMERS; i++)
        pthread_create(&consumers[i], NULL, consumer, &consumer_id[i]);

    pthread_create(&monitor, NULL, monitor_queue, NULL);

    sleep(RUN_TIME);

    pthread_mutex_lock(&lock);
    running = 0;

    pthread_cond_broadcast(&not_full);
    pthread_cond_broadcast(&not_empty);

    pthread_mutex_unlock(&lock);

    for (int i = 0; i < PRODUCERS; i++)
        pthread_join(producers[i], NULL);

    for (int i = 0; i < CONSUMERS; i++)
        pthread_join(consumers[i], NULL);

    pthread_join(monitor, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    printf("\nSimulation finished.\n");

    return 0;
}
