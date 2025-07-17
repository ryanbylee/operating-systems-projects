# Operating Systems Projects

This repository contains several small projects illustrating key concepts in operating systems.  Each project lives in its own `labX` directory.

## Lab 0 – A Kernel Seedling
A Linux kernel module that exposes `/proc/count`, printing how many processes are currently running.  Use `make` to build the module and `insmod`/`rmmod` to load and unload it.

## Lab 1 – Pipe Up
A C program implementing a simple version of the shell pipe (`|`).  After building with `make`, run `./pipe` followed by a list of commands to execute them as a pipeline.

## Lab 2 – You Spin Me Round Robin
A round‑robin scheduler simulator written in C.  Build with `make` and run `./rr <process-file> <quantum>` to compute average waiting and response times for the processes listed in the given file.

## Lab 3 – Hash Hash Hash
Thread‑safe hash table implementations in C.  Three versions are provided along with a performance tester.  Build with `make` and run `./hash-table-tester` with optional `-t` (threads) and `-s` (entries) flags.

## Lab 4 – Hey! I'm Filing Here
Creates a minimal 1 MiB ext2 filesystem image containing a few sample files and directories.  Build with `make` to produce `ext2-create`, run it to generate `cs111-base.img`, then mount the image with `sudo mount -o loop`.

Each lab directory includes its own README with more detailed instructions and unit tests.
