#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define DEPOSITORS 2
#define WITHDRAWERS 2
#define TRANSACTIONS 25

int balance = 1000;

pthread_mutex_t lock;
pthread_cond_t money_available;

void deposit(int amount)
{
    pthread_mutex_lock(&lock);

    balance += amount;
    printf("Deposited $%d | Balance = $%d\n", amount, balance);

    pthread_cond_signal(&money_available);

    pthread_mutex_unlock(&lock);
}

void withdraw(int amount)
{
    pthread_mutex_lock(&lock);

    while (balance < amount)
    {
        printf("Withdrawer waiting for $%d | Balance = $%d\n",
               amount, balance);

        pthread_cond_wait(&money_available, &lock);
    }

    balance -= amount;
    printf("Withdrawn $%d | Balance = $%d\n", amount, balance);

    pthread_mutex_unlock(&lock);
}

void *depositor(void *arg)
{
    for (int i = 0; i < TRANSACTIONS; i++)
    {
        int amount = rand() % 100 + 1;
        deposit(amount);
        usleep(100000);
    }

    return NULL;
}

void *withdrawer(void *arg)
{
    for (int i = 0; i < TRANSACTIONS; i++)
    {
        int amount = rand() % 100 + 1;
        withdraw(amount);
        usleep(100000);
    }

    return NULL;
}

int main()
{
    pthread_t depositors[DEPOSITORS];
    pthread_t withdrawers[WITHDRAWERS];

    srand(time(NULL));

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&money_available, NULL);

    for (int i = 0; i < DEPOSITORS; i++)
        pthread_create(&depositors[i], NULL, depositor, NULL);

    for (int i = 0; i < WITHDRAWERS; i++)
        pthread_create(&withdrawers[i], NULL, withdrawer, NULL);

    for (int i = 0; i < DEPOSITORS; i++)
        pthread_join(depositors[i], NULL);

    for (int i = 0; i < WITHDRAWERS; i++)
        pthread_join(withdrawers[i], NULL);

    printf("\nFinal Balance = $%d\n", balance);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&money_available);

    return 0;
}
