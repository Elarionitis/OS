#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#define SEM_TASKS "/mac_tasks_pool_v3"

int *task_queue;
int head = 0, tail = 0;
int num_workers, num_tasks;

pthread_mutex_t queue_mutex;
sem_t *tasks_available;

void* worker_thread(void* arg) {
    int worker_id = *((int*)arg);
    
    while (1) {
        sem_wait(tasks_available);
        
        pthread_mutex_lock(&queue_mutex);
        int my_task = task_queue[head];
        head++; 
        pthread_mutex_unlock(&queue_mutex);
        
        if (my_task == -1) {
            break; 
        }
        
        printf("Worker %d processing task %d\n", worker_id, my_task);
        usleep(100000); 
    }
    return NULL;
}

int main() {
    printf("Enter number of worker threads: ");
    scanf("%d", &num_workers);
    printf("Enter number of tasks: ");
    scanf("%d", &num_tasks);

    int total_queue_size = num_tasks + num_workers;
    task_queue = (int*)malloc(total_queue_size * sizeof(int));

    pthread_t *workers = (pthread_t*)malloc(num_workers * sizeof(pthread_t));
    int *worker_ids = (int*)malloc(num_workers * sizeof(int));

    pthread_mutex_init(&queue_mutex, NULL);
    
    sem_unlink(SEM_TASKS);
    tasks_available = sem_open(SEM_TASKS, O_CREAT | O_EXCL, 0644, 0);
    
    if (tasks_available == SEM_FAILED) {
        perror("Error: Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_workers; i++) {
        worker_ids[i] = i + 1;
        pthread_create(&workers[i], NULL, worker_thread, &worker_ids[i]);
    }

    for (int i = 1; i <= num_tasks; i++) {
        pthread_mutex_lock(&queue_mutex);
        task_queue[tail] = i;
        tail++;
        pthread_mutex_unlock(&queue_mutex);
        
        sem_post(tasks_available); 
        usleep(10000); 
    }

    for (int i = 0; i < num_workers; i++) {
        pthread_mutex_lock(&queue_mutex);
        task_queue[tail] = -1; 
        tail++;
        pthread_mutex_unlock(&queue_mutex);
        
        sem_post(tasks_available);
    }

    for (int i = 0; i < num_workers; i++) {
        pthread_join(workers[i], NULL);
    }

    pthread_mutex_destroy(&queue_mutex);
    sem_close(tasks_available);
    sem_unlink(SEM_TASKS);
    free(task_queue);
    free(workers);
    free(worker_ids);
    
    return 0;
}
