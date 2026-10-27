# Operating Systems — Lab Work

This repository contains my lab work, assignments, implementations, and practice programs for the **Operating Systems** course.

The main goal of this repository is to build a practical understanding of Operating System concepts by implementing and experimenting with them in C.

## 📚 Topics Covered

The labs in this repository cover:

- Process creation and management
- System calls
- Process states and synchronization
- CPU scheduling algorithms
- Inter-process communication (IPC)
- Threads and multithreading
- Process synchronization
- Deadlocks
- Memory management
- File systems and file operations
- Other Operating System concepts covered during the course

## 🧪 Labs

| Lab | Topic | Status |
|-----|-------|--------|
| Lab 2 | Processes, system calls, signals, file I/O, and `fork`/`wait` | Complete |
| Lab 3 | CPU scheduling — Round Robin | Complete |
| Lab 4 | Matrix multiplication using POSIX threads | Complete |
| Lab 5 | Mutexes, condition variables, and a bounded print queue | Complete |
| Lab 6 | Semaphores, producer-consumer synchronization, worker threads, and readers-writers | Complete |
| Lab 7 | Memory segmentation and paging (address translation) | Complete |
| Lab 9 | Paging, page replacement (FIFO/LRU/Optimal), and demand paging | Complete |

## 💻 Language & Tools

- **C**
- GCC
- Linux / Unix environment
- Git & GitHub

## 📁 Repository Structure

Each lab has its own directory:

```text
OS/
├── Lab2/
│   ├── Assignment_1/
│   └── Assignment_2/
├── Lab3/
│   ├── Round-Robin_Scheduling.c
│   └── trace_rr_problem2.txt
├── Lab4/
│   └── Matrix_Multiplication.c
├── Lab5/
│   ├── counter_no_mutex.c
│   ├── counter_mutex.c
│   ├── bank.c
│   └── print_queue.c
├── Lab6/
│   ├── Q1.c
│   ├── Q2.c
│   └── Q3.c
├── Lab7/
│   ├── Q1.c
│   ├── Q2.c
│   └── README.md
├── Lab9/
│   ├── Q1.c
│   ├── Q2.c
│   ├── Q3.c
│   └── README.md
└── README.md
```

## 🔧 Compiling

Most programs can be compiled with GCC and POSIX thread support:

```bash
gcc source_file.c -o program_name -pthread
./program_name
```

For example:

```bash
gcc Lab6/Q1.c -o Lab6/q1 -pthread
gcc Lab6/Q2.c -o Lab6/q2 -pthread
gcc Lab6/Q3.c -o Lab6/q3 -pthread
```

Lab 7 and Lab 9 programs don't use threads, so `-pthread` isn't needed:

```bash
gcc Lab7/Q1.c -o Lab7/q1
gcc Lab7/Q2.c -o Lab7/q2
gcc Lab9/Q1.c -o Lab9/q1
gcc Lab9/Q2.c -o Lab9/q2
gcc Lab9/Q3.c -o Lab9/q3
```
