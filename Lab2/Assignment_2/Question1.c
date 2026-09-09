#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pids[4];
    int expected[4] = {1275, 5040, 10, 9876};
    
    // Child 1
    pids[0] = fork();
    if (pids[0] == 0) {
        int sum = 0;
        for (int i = 1; i <= 50; i++) sum += i;
        printf("Student 1 [PID %d]: Sum = %d -> exit(%d)\n", getpid(), sum, sum % 256);
        exit(sum % 256);
    }

    // Child 2
    pids[1] = fork();
    if (pids[1] == 0) {
        int fact = 1;
        for (int i = 1; i <= 7; i++) fact *= i;
        printf("Student 2 [PID %d]: Factorial = %d -> exit(%d)\n", getpid(), fact, fact % 256);
        exit(fact % 256);
    }

    // Child 3
    pids[2] = fork();
    if (pids[2] == 0) {
        int count = 0;
        for (int i = 1; i <= 20; i++) {
            if (i % 2 == 0) count++;
        }
        printf("Student 3 [PID %d]: Even numbers = %d -> exit(%d)\n", getpid(), count, count % 256);
        exit(count % 256);
    }

    // Child 4
    pids[3] = fork();
    if (pids[3] == 0) {
        int num = 6789, rev = 0;
        while (num != 0) {
            rev = rev * 10 + num % 10;
            num /= 10;
        }
        printf("Student 4 [PID %d]: Reverse = %d -> exit(%d)\n", getpid(), rev, rev % 256);
        exit(rev % 256);
    }

    // Parent (Examiner)
    int passed = 0, status;
    printf("\n");

    for (int i = 0; i < 4; i++) {
        waitpid(pids[i], &status, 0);
        int exit_val = WEXITSTATUS(status);
        
        if (exit_val == (expected[i] % 256)) {
            printf("Examiner: Student %d -> CORRECT\n", i + 1);
            passed++;
        } else {
            printf("Examiner: Student %d -> WRONG\n", i + 1);
        }
    }

    printf("\nExaminer Report: %d/4 students passed.\n", passed);
    return 0;
}