#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N, M, P;

int **A;
int **B;
int **C;

void *multiply_row(void *arg)
{
    int row = *(int *)arg;

    for (int j = 0; j < P; j++)
    {
        C[row][j] = 0;

        for (int k = 0; k < M; k++)
        {
            C[row][j] += A[row][k] * B[k][j];
        }
    }

    return NULL;
}

int main()
{
    printf("Enter N, M and P: ");
    scanf("%d %d %d", &N, &M, &P);

    A = malloc(N * sizeof(int *));
    B = malloc(M * sizeof(int *));
    C = malloc(N * sizeof(int *));

    for (int i = 0; i < N; i++)
        A[i] = malloc(M * sizeof(int));

    for (int i = 0; i < M; i++)
        B[i] = malloc(P * sizeof(int));

    for (int i = 0; i < N; i++)
        C[i] = malloc(P * sizeof(int));

    printf("Enter matrix A:\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            scanf("%d", &A[i][j]);
    }

    printf("Enter matrix B:\n");
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < P; j++)
            scanf("%d", &B[i][j]);
    }

    pthread_t threads[N];
    int row[N];

    for (int i = 0; i < N; i++)
    {
        row[i] = i;
        pthread_create(&threads[i], NULL, multiply_row, &row[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    printf("Result matrix C:\n");

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < P; j++)
            printf("%d ", C[i][j]);

        printf("\n");
    }

    for (int i = 0; i < N; i++)
        free(A[i]);

    for (int i = 0; i < M; i++)
        free(B[i]);

    for (int i = 0; i < N; i++)
        free(C[i]);

    free(A);
    free(B);
    free(C);

    return 0;
}