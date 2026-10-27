/*
 * Lab 9 - Question 2: Page Replacement Algorithms (FIFO, LRU, Optimal)
 *
 * The idea in plain words:
 *   RAM has only a few frames. A program asks for pages one by one
 *   (the "reference string"). For each request:
 *     - If the page is already in a frame  -> HIT, nothing to do.
 *     - If it is not in any frame          -> PAGE FAULT, load it.
 *         - If there is an empty frame, put it there.
 *         - Otherwise we must kick out (replace) one page. Which one?
 *
 *   That choice is the "page replacement algorithm":
 *     FIFO    : remove the page that came in FIRST (the oldest one).
 *     LRU     : remove the page that was USED longest ago.
 *     Optimal : remove the page that will be needed furthest in the FUTURE
 *               (or never again). Best possible, but needs to know the future.
 */

#include <stdio.h>

#define MAX_REFS   100
#define MAX_FRAMES 10

int refs[MAX_REFS];     // the page reference string
int num_refs;
int num_frames;

int frames[MAX_FRAMES];     // which page is in each frame (-1 = empty)
int loaded_at[MAX_FRAMES];  // time the page was loaded   (used by FIFO)
int last_used[MAX_FRAMES];  // time the page was last used (used by LRU)

// Return the frame index holding 'page', or -1 if it is not in memory
int find_page(int page) {
    for (int i = 0; i < num_frames; i++)
        if (frames[i] == page)
            return i;
    return -1;
}

// Return the index of an empty frame, or -1 if all frames are full
int find_empty_frame() {
    for (int i = 0; i < num_frames; i++)
        if (frames[i] == -1)
            return i;
    return -1;
}

// FIFO: pick the frame whose page was loaded earliest
int pick_victim_fifo() {
    int victim = 0;
    for (int i = 1; i < num_frames; i++)
        if (loaded_at[i] < loaded_at[victim])
            victim = i;
    return victim;
}

// LRU: pick the frame whose page was used least recently
int pick_victim_lru() {
    int victim = 0;
    for (int i = 1; i < num_frames; i++)
        if (last_used[i] < last_used[victim])
            victim = i;
    return victim;
}

// Optimal: pick the frame whose page is needed furthest in the future.
// 'now' is the position of the current reference in the list.
int pick_victim_optimal(int now) {
    int victim = 0;
    int farthest = -1;

    for (int i = 0; i < num_frames; i++) {
        // Find when this page is next used
        int next_use = MAX_REFS + 1;   // "never used again"
        for (int j = now + 1; j < num_refs; j++) {
            if (refs[j] == frames[i]) {
                next_use = j;
                break;
            }
        }
        // Keep the one used furthest away
        if (next_use > farthest) {
            farthest = next_use;
            victim = i;
        }
    }
    return victim;
}

// Print what is in the frames right now, e.g. "[ 7  0  - ]"
void print_frames() {
    printf("[");
    for (int i = 0; i < num_frames; i++) {
        if (frames[i] == -1) printf("  -");
        else                 printf(" %2d", frames[i]);
    }
    printf(" ]");
}

// Run the chosen algorithm on the reference string and count faults.
// algo: 1 = FIFO, 2 = LRU, 3 = Optimal
int simulate(int algo) {
    int faults = 0;

    // Start with all frames empty
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
        loaded_at[i] = 0;
        last_used[i] = 0;
    }

    printf("\nRef   Frames");
    for (int i = 1; i < num_frames; i++) printf("   ");
    printf("  Result\n");

    for (int t = 0; t < num_refs; t++) {
        int page = refs[t];
        int pos = find_page(page);

        printf("%-5d ", page);

        if (pos != -1) {
            // HIT: page is already in memory, just mark it as used now
            last_used[pos] = t;
            print_frames();
            printf("  Hit\n");
            continue;
        }

        // PAGE FAULT: page is not in memory
        faults++;

        // Use an empty frame if there is one, otherwise choose a victim
        pos = find_empty_frame();
        if (pos == -1) {
            if (algo == 1)      pos = pick_victim_fifo();
            else if (algo == 2) pos = pick_victim_lru();
            else                pos = pick_victim_optimal(t);
        }

        // Load the new page into the chosen frame
        frames[pos] = page;
        loaded_at[pos] = t;
        last_used[pos] = t;

        print_frames();
        printf("  Fault\n");
    }

    return faults;
}

int main() {
    int algo;

    // Input 1: the page reference string
    printf("Enter number of page references (max %d): ", MAX_REFS);
    scanf("%d", &num_refs);
    if (num_refs < 1 || num_refs > MAX_REFS) {
        printf("Invalid number of references.\n");
        return 1;
    }
    printf("Enter the page references (e.g. 7 0 1 2 0 3 0 4 2 3): ");
    for (int i = 0; i < num_refs; i++)
        scanf("%d", &refs[i]);

    // Input 2: number of frames
    printf("Enter number of frames (max %d): ", MAX_FRAMES);
    scanf("%d", &num_frames);
    if (num_frames < 1 || num_frames > MAX_FRAMES) {
        printf("Invalid number of frames.\n");
        return 1;
    }

    // Input 3: which algorithm
    printf("Choose algorithm: 1 = FIFO, 2 = LRU, 3 = Optimal: ");
    scanf("%d", &algo);
    if (algo < 1 || algo > 3) {
        printf("Invalid choice.\n");
        return 1;
    }

    const char *names[] = {"", "FIFO", "LRU", "Optimal"};
    printf("\n===== %s Page Replacement =====", names[algo]);

    int faults = simulate(algo);

    printf("\nTotal page faults (%s): %d\n", names[algo], faults);
    printf("Total hits: %d\n", num_refs - faults);
    return 0;
}
