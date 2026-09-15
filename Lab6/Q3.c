#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#define SEM_RES "/mac_res_access_final_q3"

sem_t *resource_access; 
pthread_mutex_t read_count_mutex; 
int read_count = 0;

void* reader(void* arg) {
    int reader_id = *((int*)arg);
    
    pthread_mutex_lock(&read_count_mutex);
    read_count++;
    if (read_count == 1) {
        sem_wait(resource_access);
    }
    pthread_mutex_unlock(&read_count_mutex);
    
    printf("Reader %d is reading the file\n", reader_id);
    usleep(500000); // Stay reading long enough for the writer to arrive and wait
    printf("Reader %d finished reading the file\n", reader_id);
    
    pthread_mutex_lock(&read_count_mutex);
    read_count--;
    if (read_count == 0) {
        sem_post(resource_access);
    }
    pthread_mutex_unlock(&read_count_mutex);
    
    return NULL;
}

void* writer(void* arg) {
    int writer_id = *((int*)arg);
    
    printf("Writer %d is waiting to write to the file\n", writer_id);
    sem_wait(resource_access);
    
    printf("Writer %d is writing to the file\n", writer_id);
    usleep(200000); 
    printf("Writer %d finished writing to the file\n", writer_id);
    
    sem_post(resource_access);
    
    return NULL;
}

int main() {
    // Exactly 3 readers and 1 writer to match the example screenshot
    pthread_t r[3], w[1];
    int r_ids[3] = {1, 2, 3};
    int w_ids[1] = {1};

    pthread_mutex_init(&read_count_mutex, NULL);
    
    sem_unlink(SEM_RES);
    resource_access = sem_open(SEM_RES, O_CREAT | O_EXCL, 0644, 1);
    
    if (resource_access == SEM_FAILED) {
        perror("Error: Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }

    // Stagger thread creation slightly to force the expected print order
    pthread_create(&r[0], NULL, reader, &r_ids[0]);
    usleep(10000); 
    pthread_create(&r[1], NULL, reader, &r_ids[1]);
    usleep(10000); 
    pthread_create(&r[2], NULL, reader, &r_ids[2]);
    usleep(50000); 
    
    // Writer arrives while all 3 readers are busy reading
    pthread_create(&w[0], NULL, writer, &w_ids[0]); 

    pthread_join(r[0], NULL);
    pthread_join(r[1], NULL);
    pthread_join(r[2], NULL);
    pthread_join(w[0], NULL);

    pthread_mutex_destroy(&read_count_mutex);
    sem_close(resource_access);
    sem_unlink(SEM_RES);
    
    return 0;
}
