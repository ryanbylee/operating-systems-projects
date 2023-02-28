## UID: 905753127

# Hash Hash Hash

C Hash Table implementation with pthread library that can safely be used concurrently with multiple threads.

## Building

To build the executable, run 

`make`

## Running

To run the executable, run

`./hash-table-tester` with optional `-t` flag or `-s` flag.

`-t` flag specifies the number of threads to use when running the testing script.

`-s` flag specifies the number of entries to insert to hash table when running the testing script.

e.g. `./hash-table-tester -t 4 -s 25000` would output
```
Generation: 42,904 usec
Hash table base: 166,593 usec
  - 0 missing
Hash table v1: 310,588 usec
  - 0 missing
Hash table v2: 41,790 usec
  - 0 missing
```

## First Implementation

The first implementation in hash-table-v1.c ensures the correctness when being executed concurrently by locking the entirety of `hash_table_v1_add_entry` function, which contains a critical section, with a single mutex.

Since the mutex only allows one thread at a time to access the hash table when inserting a new entry, it eliminates data race condition and ensures correctness.

### Performance

When running with one thread, the tester outputs

```
./hash-table-tester -t 1 -s 100000
Generation: 26,574 usec
Hash table base: 144,358 usec
  - 0 missing
Hash table v1: 123,788 usec
  - 0 missing
```
with a non-significant change in completion time.

Since the work of one thread is not affected by the add-entry function being locked/unlocked, the completion times of base/v1 is expected to be similar.

When running with four thread, however, the tester outputs

```
./hash-table-tester -t 4 -s 25000
Generation: 28,431 usec
Hash table base: 145,535 usec
  - 0 missing
Hash table v1: 363,215 usec
  - 0 missing
```
with a slow-down of 2.5x compared to the base version.

Since only one thread is allowed to access the add-entry function at a single time, other three threads must wait until the code segment is unlocked by the other thread. With the added overhead of mutex lock/unlock, the completion time of v1 with higher threads is expected to be much slower than that of base version.


## Second Implementation

Describe your second implementation strategy here (the one with a multiple
mutexes). Argue why your strategy is correct.

### Performance

Run the tester such that the base hash table completes in 1-2 seconds.
Report the relative speedup with number of threads equal to the number of
physical cores on your machine (at least 4). Note again that the amount of work
(`-t` times `-s`) should remain constant. Report the speedup relative to the
base hash table implementation. Explain the difference between this
implementation and your first, which respect why you get a performance increase.

## Cleaning up

Explain briefly how to clean up all binary files.
