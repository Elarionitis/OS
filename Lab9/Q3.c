/*
 * Lab 9 - Question 3: Demand Paging with FIFO Page Replacement
 *
 * The idea in plain words:
 *   "Demand paging" means a page is NOT loaded into memory until the
 *   process actually asks for it. So at the start every process has
 *   nothing in memory, and the first time it touches a page we get a
 *   PAGE FAULT and load that page.
 *
 *   Each process gets the same number of physical frames (e.g. 3).
 *   When its frames are full and a new page is needed, we remove the
 *   OLDEST page that was loaded (FIFO = First In, First Out).
 *
 *   FIFO is easy to do with a "pointer" that goes round the frames:
 *     frames:  [ A  B  C ]     next_out = 0  -> A is the oldest
 *     load D:  [ D  B  C ]     next_out = 1  -> now B is the oldest
 *     load E:  [ D  E  C ]     next_out = 2  -> now C is the oldest
 *   When next_out reaches the end it goes back to 0.
 */

#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_REFS      50
#define MAX_FRAMES    10

// Everything we need to remember about one process
struct Process {
    int refs[MAX_REFS];      // its page reference sequence
    int num_refs;
    int frames[MAX_FRAMES];  // pages it currently has in memory (-1 = empty)
    int next_out;            // FIFO pointer: frame that holds the oldest page
    int faults;              // page faults for this process
};

struct Process proc[MAX_PROCESSES];
int num_processes;
int num_frames;

// Is 'page' already in this process's frames?
int in_memory(struct Process *p, int page) {
    for (int i = 0; i < num_frames; i++)
        if (p->frames[i] == page)
            return 1;
    return 0;
}

// Print the frames of a process, e.g. "[  0  4  - ]"
void print_frames(struct Process *p) {
    printf("[");
    for (int i = 0; i < num_frames; i++) {
        if (p->frames[i] == -1) printf("  -");
        else                    printf(" %2d", p->frames[i]);
    }
    printf(" ]");
}

// Run demand paging + FIFO for one process
void run_process(int id) {
    struct Process *p = &proc[id];

    // Demand paging: memory starts EMPTY for every process
    for (int i = 0; i < num_frames; i++)
        p->frames[i] = -1;
    p->next_out = 0;
    p->faults = 0;

    printf("\n===== Process P%d =====\n", id + 1);
    printf("Ref   Memory");
    for (int i = 1; i < num_frames; i++) printf("   ");
    printf("  Result\n");

    for (int t = 0; t < p->num_refs; t++) {
        int page = p->refs[t];
        printf("%-5d ", page);

        if (in_memory(p, page)) {
            // Page is already loaded, nothing to do
            print_frames(p);
            printf("  Hit\n");
            continue;
        }

        // PAGE FAULT: load the page on demand.
        // The slot at next_out is either empty (memory not full yet)
        // or holds the oldest page, which FIFO replaces.
        p->faults++;
        int old = p->frames[p->next_out];
        p->frames[p->next_out] = page;
        p->next_out = (p->next_out + 1) % num_frames;   // move FIFO pointer

        print_frames(p);
        if (old == -1) printf("  Fault (loaded into free frame)\n");
        else           printf("  Fault (replaced page %d)\n", old);
    }

    printf("Page faults for P%d: %d\n", id + 1, p->faults);
}

int main() {
    // Input 1: number of frames
    printf("Enter number of physical frames per process (max %d): ", MAX_FRAMES);
    scanf("%d", &num_frames);
    if (num_frames < 1 || num_frames > MAX_FRAMES) {
        printf("Invalid number of frames.\n");
        return 1;
    }

    // Input 2: the processes and their page reference sequences
    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &num_processes);
    if (num_processes < 1 || num_processes > MAX_PROCESSES) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (int i = 0; i < num_processes; i++) {
        printf("\nP%d - number of page references (max %d): ", i + 1, MAX_REFS);
        scanf("%d", &proc[i].num_refs);
        if (proc[i].num_refs < 1 || proc[i].num_refs > MAX_REFS) {
            printf("Invalid number of references.\n");
            return 1;
        }
        printf("P%d - enter the page references: ", i + 1);
        for (int j = 0; j < proc[i].num_refs; j++)
            scanf("%d", &proc[i].refs[j]);
    }

    // Simulate each process
    for (int i = 0; i < num_processes; i++)
        run_process(i);

    // Final summary
    int total = 0;
    printf("\n===== Summary =====\n");
    for (int i = 0; i < num_processes; i++) {
        printf("P%d: %d page faults\n", i + 1, proc[i].faults);
        total += proc[i].faults;
    }
    printf("Total page faults: %d\n", total);
    return 0;
}
