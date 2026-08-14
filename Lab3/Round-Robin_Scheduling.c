#include <stdio.h>

#define MAX 100

typedef struct {
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
} Process;

typedef struct {
    int pid;
    int start;
    int end;
} Gantt;

void round_robin(Process proc[], int n, int quantum) {

    int rem_bt[MAX];

    for (int i = 0; i < n; i++) {
        rem_bt[i] = proc[i].bt;
    }

    int time = 0;
    int completed = 0;

    Gantt gantt[MAX * 10];
    int g = 0;

    while (completed < n) {

        int any = 0;

        for (int i = 0; i < n; i++) {

            if (rem_bt[i] > 0 && proc[i].at <= time) {

                any = 1;

                int start = time;

                if (rem_bt[i] > quantum) {

                    time += quantum;
                    rem_bt[i] -= quantum;

                } else {

                    time += rem_bt[i];
                    rem_bt[i] = 0;

                    proc[i].ct = time;
                    proc[i].tat = proc[i].ct - proc[i].at;
                    proc[i].wt = proc[i].tat - proc[i].bt;

                    completed++;
                }

                gantt[g].pid = proc[i].pid;
                gantt[g].start = start;
                gantt[g].end = time;
                g++;
            }
        }

        // If no process has arrived, CPU remains idle.
        if (!any) {
            time++;
        }
    }

    printf("\nGantt Chart:\n");

    for (int i = 0; i < g; i++) {
        printf("| P%d ", gantt[i].pid);
    }
    printf("|\n");

    printf("%d", gantt[0].start);

    for (int i = 0; i < g; i++) {
        printf("    %d", gantt[i].end);
    }

    printf("\n\n");

    printf("Process\tAT\tBT\tCT\tTAT\tWT\n");

    float total_wt = 0;
    float total_tat = 0;

    for (int i = 0; i < n; i++) {

        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               proc[i].pid,
               proc[i].at,
               proc[i].bt,
               proc[i].ct,
               proc[i].tat,
               proc[i].wt);

        total_wt += proc[i].wt;
        total_tat += proc[i].tat;
    }

    printf("\nAverage Waiting Time = %.2f\n", total_wt / n);
    printf("Average Turnaround Time = %.2f\n", total_tat / n);
}

int main() {

    Process proc[MAX];

    int n;
    int quantum;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {

        proc[i].pid = i + 1;

        printf("Enter arrival time and burst time for P%d: ",
               i + 1);

        scanf("%d %d", &proc[i].at, &proc[i].bt);
    }

    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    round_robin(proc, n, quantum);

    return 0;
}