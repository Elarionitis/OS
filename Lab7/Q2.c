/*
 * Lab 7 - Question 2: Paging (Virtual -> Physical Address)
 *
 * The idea in plain words:
 *   Memory is cut into equal-sized blocks.
 *     - In virtual memory these blocks are called PAGES.
 *     - In physical memory (RAM) they are called FRAMES.
 *   The page table tells us which frame each page lives in.
 *
 *   To translate a virtual address:
 *     page number = virtual address / page size
 *     offset      = virtual address % page size
 *     frame       = page_table[page number]
 *     physical    = frame * page size + offset
 *
 *   If the page is not in the page table (marked -1), that is a PAGE FAULT.
 */

#include <stdio.h>

#define NUM_PAGES 8   // our small virtual memory has 8 pages

// Hardcoded page table: page_table[page] = frame number.
// -1 means the page is not loaded in memory (not mapped).
int page_table[NUM_PAGES] = {
    5,    // page 0 -> frame 5
    2,    // page 1 -> frame 2
    -1,   // page 2 -> not mapped
    7,    // page 3 -> frame 7
    0,    // page 4 -> frame 0
    -1,   // page 5 -> not mapped
    3,    // page 6 -> frame 3
    1     // page 7 -> frame 1
};

// Show the page table so the user knows what is mapped
void print_page_table() {
    printf("\n----- Page Table -----\n");
    printf("Page   Frame\n");
    for (int i = 0; i < NUM_PAGES; i++) {
        if (page_table[i] == -1)
            printf("%-6d (not mapped)\n", i);
        else
            printf("%-6d %d\n", i, page_table[i]);
    }
    printf("----------------------\n");
}

int main() {
    unsigned long page_size;

    printf("Enter page size in bytes (e.g. 4096 for 4KB): ");
    scanf("%lu", &page_size);

    if (page_size == 0) {
        printf("Page size must be greater than 0.\n");
        return 1;
    }

    print_page_table();
    printf("Valid virtual addresses: 0 to %lu\n", NUM_PAGES * page_size - 1);

    // Keep translating until the user enters -1
    while (1) {
        long input;

        // %li accepts both decimal (e.g. 8200) and hex (e.g. 0x2008)
        printf("\nEnter virtual address in decimal or hex (-1 to quit): ");
        if (scanf("%li", &input) != 1 || input == -1) break;

        if (input < 0) {
            printf("Address cannot be negative.\n");
            continue;
        }
        unsigned long virtual_addr = (unsigned long)input;

        // Step 1: split the address into page number and offset
        unsigned long page   = virtual_addr / page_size;
        unsigned long offset = virtual_addr % page_size;
        printf("Virtual address %lu (0x%lX) -> page %lu, offset %lu\n",
               virtual_addr, virtual_addr, page, offset);

        // Step 2: look up the page in the page table
        if (page >= NUM_PAGES || page_table[page] == -1) {
            printf("PAGE FAULT: page %lu is not mapped to any frame.\n", page);
            continue;
        }
        unsigned long frame = page_table[page];

        // Step 3: combine frame number and offset
        unsigned long physical_addr = frame * page_size + offset;
        printf("Page %lu is in frame %lu\n", page, frame);
        printf("Virtual address %lu (0x%lX) -> Physical address %lu (0x%lX)\n",
               virtual_addr, virtual_addr, physical_addr, physical_addr);
    }

    printf("Goodbye!\n");
    return 0;
}
