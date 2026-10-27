# Lab 9 — Paging, Page Replacement and Demand Paging

## Q1 — Paging with a Page Table (`Q1.c`)

**Idea:** Virtual memory is cut into equal blocks called **pages**. Physical memory is cut into blocks of the same size called **frames**.
The **page table** tells us which frame holds each page.

```
page number = virtual address / page size
offset      = virtual address % page size
frame       = page_table[page number]
physical    = frame * page size + offset
```

If the page table has `-1` for that page (or the page number is too big), it's a **Page Fault**.

**Inputs:** the page size, the page table (use the built-in 8-page table or type your own), and then virtual addresses in decimal (`12304`) or hex (`0x3010`). Enter `-1` to quit.

**Sample run (page size 4096, default table):**

```
0x3010 -> page 3, offset 16 -> frame 7 -> Physical 0x7010
0x2008 -> page 2 -> PAGE FAULT (not mapped)
40000  -> page 9 -> PAGE FAULT (outside page table)
```

## Q2 — Page Replacement: FIFO, LRU, Optimal (`Q2.c`)

**Idea:** RAM has only a few frames. For each page in the reference list:

- Already in a frame → **Hit**
- Not in any frame → **Page Fault**. Put it in an empty frame. If none are empty, remove one page:
  - **FIFO**: remove the page that was loaded **first**
  - **LRU**: remove the page that was **used longest ago**
  - **Optimal**: remove the page that won't be needed for the **longest time** in the future

The program shows the frames after every reference and the total number of faults.

**Sample (references `7 0 1 2 0 3 0 4 2 3`, 3 frames):**

| Algorithm | Page faults |
|-----------|-------------|
| FIFO      | 9 |
| LRU       | 8 |
| Optimal   | 6 |

## Q3 — Demand Paging with FIFO (`Q3.c`)

**Idea:** With *demand paging*, a page is loaded only when a process asks for it, so every process starts with empty memory.
Each process gets the same number of frames. When its frames are full, the **oldest** page is replaced (FIFO).

FIFO is done with a pointer `next_out` that goes round the frames: it always points at the oldest page, and after each load it moves forward by one (going back to 0 at the end).

**Sample (3 frames, P1: `0 4 1 0 3`, P2: `2 0 3 1 2`):**

```
P1: 0 -> [0 - -] Fault   4 -> [0 4 -] Fault   1 -> [0 4 1] Fault
    0 -> [0 4 1] Hit     3 -> [3 4 1] Fault (replaced 0)      => 4 faults
P2: 2 -> [2 - -] Fault   0 -> [2 0 -] Fault   3 -> [2 0 3] Fault
    1 -> [1 0 3] Fault   2 -> [1 2 3] Fault (replaced 0)      => 5 faults
```

## How to run

```bash
gcc Q1.c -o q1 && ./q1
gcc Q2.c -o q2 && ./q2
gcc Q3.c -o q3 && ./q3
```
