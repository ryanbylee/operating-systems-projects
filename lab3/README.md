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

The second implementation in hash-table-v2 ensures both correctness and improved performance by setting the grangularity of locking to each bucket. Each hash table entry has a mutex, and when a thread accesses the entry, it instantly locks the entry, blocking other threads to access the same bucket. Since the data race condition occurs when more than one thread attempt to insert an entry into the same bucket in the hash table, locking each bucket eliminates the data race condition while allowing each thread to work concurrently on different buckets.

### Performance

When running with one thread, the tester outputs

```
./hash-table-tester -t 1 -s 100000
Generation: 20,159 usec
Hash table base: 82,122 usec
  - 0 missing
Hash table v1: 98,009 usec
  - 0 missing
Hash table v2: 53,304 usec
  - 0 missing
```

which does not result in a significant speed-up of v2 compared to the base version, since only one thread would serially insert the entries for both v2 and the base version.

When running with four thread (The machine has four cores), however, the tester outputs

```
./hash-table-tester -t 4 -s 25000
Generation: 24,217 usec
Hash table base: 73,990 usec
  - 0 missing
Hash table v1: 152,144 usec
  - 0 missing
Hash table v2: 23,845 usec
  - 0 missing
```

with a significant speed-up of about 3x compared to the completion time of the base version.

The difference between the implementations of v1 and v2 is that while v1 locks the entire add_entry function, not allowing multiple threads to insert entries at the same time, v2 locks each bucket in the hash table, allowing threads to concurrently insert entries as long as they are not inserting in the same bucket at the same time. This difference allows parallelism in v2, resulting in a significant speed-up compared to the base version.



## Cleaning up

To clean up the binary files, run

`make clean`