# Lab 7 — Memory Segmentation and Paging

## Q1 — Memory Segmentation (`Q1.c`)

**Idea:** A program is split into parts called *segments* (code, data, stack…).
Each segment is placed in physical memory. For each one we store:

- **Base**: where it starts in memory
- **Limit**: how big it is
- **Permissions**: read (r), write (w), execute (x)

A logical address is `(segment number, offset)`, and the program turns it into a real address like this:

```
physical address = base + offset
```

Before doing that, it runs three checks:

1. The segment doesn't exist → **Segmentation Fault**
2. The offset is ≥ the segment size → **Segmentation Fault**
3. The operation isn't allowed (e.g. writing to code) → **Protection Fault**

Physical memory is 1000 bytes. Segments are placed one after another.

**Sample run:**

```
Seg#  Name        Base   Limit  Permissions
0     code        0      300    r-x
1     data        300    200    rw-
2     stack       500    100    rw-

(0, 50,  x) -> Physical address 50
(1, 199, w) -> Physical address 499
(0, 10,  w) -> Protection Fault (code is not writable)
(2, 100, r) -> Segmentation Fault (offset outside stack)
(5, 0,   r) -> Segmentation Fault (segment 5 does not exist)
```

## Q2 — Paging (`Q2.c`)

**Idea:** Virtual memory is cut into equal blocks called **pages**. Physical memory is cut into blocks of the same size called **frames**.
The **page table** tells us which frame holds each page.

```
page number = virtual address / page size
offset      = virtual address % page size
frame       = page_table[page number]
physical    = frame * page size + offset
```

If the page table has `-1` for that page (or the page number is too big), it's a **Page Fault**.

The page table is hardcoded (8 pages). You can type addresses in decimal (`12304`) or hex (`0x3010`).

**Sample run (page size 4096):**

```
0x3010 -> page 3, offset 16 -> frame 7 -> Physical 0x7010
0x2008 -> page 2 -> PAGE FAULT (not mapped)
40000  -> page 9 -> PAGE FAULT (outside page table)
```

## How to run

```bash
gcc Q1.c -o q1 && ./q1
gcc Q2.c -o q2 && ./q2
```
