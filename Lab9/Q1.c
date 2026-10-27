/*
 * Lab 9 - Question 1: Simulating Paging with a Page Table
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
 *   If the page has no frame (marked -1) or is outside the table,
 *   that is a PAGE FAULT.
 */

#include <stdio.h>

#define MAX_PAGES 64   // the most pages the user can enter

int page_table[MAX_PAGES];   // page_table[page] = frame number (-1 = not mapped)
int num_pages = 0;           // how many pages are actually in the table

// Fill the page table with a fixed example (used if the user doesn't want to type one)
void load_default_table() {
    int example[8] = {5, 2, -1, 7, 0, -1, 3, 1};
    num_pages = 8;
    for (int i = 0; i < num_pages; i++)
        page_table[i] = example[i];
}

// Let the user type their own page table
void read_table_from_user() {
    printf("How many pages in the page table (1-%d)? ", MAX_PAGES);
    scanf("%d", &num_pages);
    if (num_pages < 1 || num_pages > MAX_PAGES) {
        printf("Invalid number, using the default table instead.\n");
        load_default_table();
        return;
    }
    printf("Enter the frame number for each page (-1 if not mapped):\n");
    for (int i = 0; i < num_pages; i++) {
        printf("  Page %d -> frame: ", i);
        scanf("%d", &page_table[i]);
    }
}

// Show the page table so the user knows what is mapped
void print_page_table() {
    printf("\n----- Page Table -----\n");
    printf("Page   Frame\n");
    for (int i = 0; i < num_pages; i++) {
        if (page_table[i] == -1)
            printf("%-6d (not mapped)\n", i);
        else
            printf("%-6d %d\n", i, page_table[i]);
    }
    printf("----------------------\n");
}

int main() {
    unsigned long page_size;
    int choice;

    // Input 1: page size
    printf("Enter page size in bytes (e.g. 4096 for 4KB): ");
    scanf("%lu", &page_size);
    if (page_size == 0) {
        printf("Page size must be greater than 0.\n");
        return 1;
    }

    // Input 2: page table (hardcoded or typed by the user)
    printf("1. Use the default page table\n");
    printf("2. Enter my own page table\n");
    printf("Choice: ");
    scanf("%d", &choice);
    if (choice == 2)
        read_table_from_user();
    else
        load_default_table();

    print_page_table();
    printf("Valid virtual addresses: 0 to %lu\n", num_pages * page_size - 1);

    // Input 3: virtual addresses. Keep translating until the user enters -1.
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
        if (page >= (unsigned long)num_pages || page_table[page] == -1) {
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
