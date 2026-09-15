#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

// Unique names so we don't hit old cached semaphores
#define S1_NAME "/mac_stage1_final"
#define S2_NAME "/mac_stage2_final"
#define S3_NAME "/mac_stage3_final"

sem_t *sem_stage1, *sem_stage2, *sem_stage3;
int current_data = 0;
int *input_data;
int num_items;

void* stage1(void* arg) {
    for (int i = 0; i < num_items; i++) {
        sem_wait(sem_stage1);
        current_data = input_data[i];
        printf("Stage 1: Read data item %d\n", current_data);
        sem_post(sem_stage2);
    }
    return NULL;
}

void* stage2(void* arg) {
    for (int i = 0; i < num_items; i++) {
        sem_wait(sem_stage2);
        printf("Stage 2: Processed data item %d\n", current_data);
        sem_post(sem_stage3);
    }
    return NULL;
}

void* stage3(void* arg) {
    for (int i = 0; i < num_items; i++) {
        sem_wait(sem_stage3);
        printf("Stage 3: Output data item %d\n", current_data);
        sem_post(sem_stage1);
    }
    return NULL;
}

int main() {
    printf("Enter number of data items: ");
    scanf("%d", &num_items);
    input_data = (int*)malloc(num_items * sizeof(int));
    
    printf("Enter %d data items:\n", num_items);
    for(int i = 0; i < num_items; i++) {
        scanf("%d", &input_data[i]);
    }

    // 1. Unlink first
    sem_unlink(S1_NAME);
    sem_unlink(S2_NAME);
    sem_unlink(S3_NAME);

    // 2. Open with O_CREAT | O_EXCL to GUARANTEE fresh creation
    sem_stage1 = sem_open(S1_NAME, O_CREAT | O_EXCL, 0644, 1);
    sem_stage2 = sem_open(S2_NAME, O_CREAT | O_EXCL, 0644, 0);
    sem_stage3 = sem_open(S3_NAME, O_CREAT | O_EXCL, 0644, 0);

    // 3. Safety check to ensure they initialized correctly
    if (sem_stage1 == SEM_FAILED || sem_stage2 == SEM_FAILED || sem_stage3 == SEM_FAILED) {
        perror("Error: Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }

    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, stage1, NULL);
    pthread_create(&t2, NULL, stage2, NULL);
    pthread_create(&t3, NULL, stage3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    // 4. Proper cleanup
    sem_close(sem_stage1);
    sem_close(sem_stage2);
    sem_close(sem_stage3);
    
    sem_unlink(S1_NAME);
    sem_unlink(S2_NAME);
    sem_unlink(S3_NAME);
    
    free(input_data);
    return 0;
}