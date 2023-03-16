## UID: 905753127

# Hey! I'm Filing Here

A 1MiB ext2 file system with 2 directories (~, lost+found), 1 regular file (hello-world), and 1 symbolic link (hello)

## Building

To build the program, type 

`run`

on unix-based terminal

## Running

After the compilation, cs111-base.img will be created.

To mount the ext2 file system, run

`sudo mount -o loop cs111-base.img mnt`

The example output of

`ls -ain`

is the following:

```
total 7
2 drwxr -xr-x 3 0 0 1024 .
..
13 lrw-r--r-- 1 1000 1000 11 hello -> hello -world
12 -rw-r--r-- 1 1000 1000 12 hello -world
11 drwxr -xr-x 2 0 0 1024 lost+found
```

## Cleaning up

Explain briefly how to unmount the filesystem, remove the mount directory, and
clean up all binary files.

To unmount the filesystem and delete the directory used for mounting, run

`sudo umount mnt`

`rmdir mnt`

to clean up all binary files, run

`make clean`