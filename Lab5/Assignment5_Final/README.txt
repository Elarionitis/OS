# CSL 3030 Operating Systems Lab - Assignment V

Files:
1. counter_no_mutex.c  - Task 1, without mutex
2. counter_mutex.c     - Task 1, with mutex
3. counter_print.c     - Task 1, mutex + thread ID/counter output
4. bank.c              - Task 2, bank account with mutex + condition variable
5. print_queue.c       - Task 3, bounded print queue

## Compile

gcc counter_no_mutex.c -o counter_no_mutex -pthread
gcc counter_mutex.c -o counter_mutex -pthread
gcc counter_print.c -o counter_print -pthread
gcc bank.c -o bank -pthread
gcc print_queue.c -o print_queue -pthread

## Run

./counter_no_mutex
./counter_mutex
./counter_print
./bank
./print_queue

Task 1:
- 10 threads
- 500,000 increments per thread
- Without mutex, final value can be less than 5,000,000 because of a race condition.
- With mutex, final value is 5,000,000.
- counter_print.c prints the thread ID and counter after each increment.

Task 2:
- Initial balance = $1000
- 2 depositors + 2 withdrawers
- 25 transactions per thread = 100 transactions
- Deposits/withdrawals are $1-$100.
- Withdrawers wait on a condition variable when funds are insufficient.

Task 3:
- Circular queue capacity = 10
- 3 producers + 2 consumers + 1 monitor
- not_full is used by producers.
- not_empty is used by consumers.
- Monitor reports queue status every 2 seconds.
- Simulation runs for 30 seconds and then wakes waiting threads for graceful termination.

Note:
counter_print.c prints 5,000,000 lines, so it is normal for the terminal output to be very large.
