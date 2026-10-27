/*
 * Lab 7 - Question 1: Memory Segmentation Simulator
 *
 * The idea in plain words:
 *   A program is split into "segments" (code, data, stack, ...).
 *   Each segment is placed somewhere in physical memory. We remember
 *   where it starts (base) and how big it is (limit) in a segment table.
 *
 *   A logical address is a pair: (segment number, offset).
 *   To find the real (physical) address we simply do:
 *
 *        physical address = base of that segment + offset
 *
 *   But first we check:
 *     1. Does the segment exist?            -> if not, segmentation fault
 *     2. Is the offset inside the segment?  -> if not, segmentation fault
 *     3. Is the operation allowed (r/w/x)?  -> if not, protection fault
 */

#include <stdio.h>
#include <string.h>

#define PHYSICAL_MEMORY_SIZE 1000   // total physical memory (in bytes)
#define MAX_SEGMENTS 10             // most segments we allow

// One row of the segment table
struct Segment {
    char name[20];   // e.g. "code", "data", "stack"
    int base;        // starting address in physical memory
    int limit;       // size of the segment
    int can_read;    // 1 = allowed, 0 = not allowed
    int can_write;
    int can_execute;
};

struct Segment table[MAX_SEGMENTS];
int num_segments = 0;

// Print the segment table so the user can see where everything went
void print_segment_table() {
    printf("\n---------------- Segment Table ----------------\n");
    printf("Seg#  Name        Base   Limit  Permissions\n");
    for (int i = 0; i < num_segments; i++) {
        printf("%-5d %-11s %-6d %-6d %c%c%c\n",
               i, table[i].name, table[i].base, table[i].limit,
               table[i].can_read    ? 'r' : '-',
               table[i].can_write   ? 'w' : '-',
               table[i].can_execute ? 'x' : '-');
    }
    printf("-----------------------------------------------\n");
}

// Translate (segment, offset) into a physical address.
// 'op' is the operation the user wants: 'r' = read, 'w' = write, 'x' = execute.
void translate(int seg, int offset, char op) {
    // Check 1: the segment number must exist
    if (seg < 0 || seg >= num_segments) {
        printf("Segmentation Fault: segment %d does not exist.\n", seg);
        return;
    }

    // Check 2: the offset must be inside the segment (0 to limit-1)
    if (offset < 0 || offset >= table[seg].limit) {
        printf("Segmentation Fault: offset %d is outside segment '%s' (size %d).\n",
               offset, table[seg].name, table[seg].limit);
        return;
    }

    // Check 3: the segment must allow the requested operation
    int allowed = 0;
    if (op == 'r') allowed = table[seg].can_read;
    else if (op == 'w') allowed = table[seg].can_write;
    else if (op == 'x') allowed = table[seg].can_execute;
    else {
        printf("Unknown operation '%c'. Use r, w or x.\n", op);
        return;
    }

    if (!allowed) {
        printf("Protection Fault: '%c' is not allowed on segment '%s'.\n",
               op, table[seg].name);
        return;
    }

    // All checks passed, so do the actual translation
    int physical = table[seg].base + offset;
    printf("Logical (%d, %d) -> Physical address %d   [%s, '%c' allowed]\n",
           seg, offset, physical, table[seg].name, op);
}

int main() {
    int next_free = 0;   // next free spot in physical memory

    printf("Physical memory size: %d bytes\n", PHYSICAL_MEMORY_SIZE);
    printf("Enter number of segments (max %d): ", MAX_SEGMENTS);
    scanf("%d", &num_segments);

    if (num_segments < 1 || num_segments > MAX_SEGMENTS) {
        printf("Invalid number of segments.\n");
        return 1;
    }

    // Read every segment from the user and place it in memory one after another
    for (int i = 0; i < num_segments; i++) {
        char perms[4];

        printf("\nSegment %d\n", i);
        printf("  Name (e.g. code/data/stack): ");
        scanf("%19s", table[i].name);
        printf("  Size: ");
        scanf("%d", &table[i].limit);
        printf("  Permissions (any of r, w, x, e.g. rx or rw): ");
        scanf("%3s", perms);

        // Make sure the segment fits in the remaining memory
        if (table[i].limit <= 0 || next_free + table[i].limit > PHYSICAL_MEMORY_SIZE) {
            printf("  Not enough memory for this segment. Stopping here.\n");
            num_segments = i;   // keep only the segments that fit
            break;
        }

        // Allocate: this segment starts where the last one ended
        table[i].base = next_free;
        next_free += table[i].limit;

        // Turn the permission string into three yes/no flags
        table[i].can_read    = (strchr(perms, 'r') != NULL);
        table[i].can_write   = (strchr(perms, 'w') != NULL);
        table[i].can_execute = (strchr(perms, 'x') != NULL);
    }

    print_segment_table();
    printf("Memory used: %d / %d bytes\n", next_free, PHYSICAL_MEMORY_SIZE);

    // Keep translating addresses until the user enters -1
    while (1) {
        int seg, offset;
        char op;

        printf("\nEnter segment number (-1 to quit): ");
        scanf("%d", &seg);
        if (seg == -1) break;

        printf("Enter offset: ");
        scanf("%d", &offset);
        printf("Enter operation (r/w/x): ");
        scanf(" %c", &op);   // the space skips the leftover newline

        translate(seg, offset, op);
    }

    printf("Goodbye!\n");
    return 0;
}
