# A Kernel Seedling

Kernel Module to print the number of currnetly-running processes

## Building

Explain how to build your kernel module
To build the kernel module, run
`make`
inside the directory

## Running

To load the kernel module into kernel, run
`sudo insmod proc_count.ko`

To check if the module is loaded successfully, run
`lsmod`

To run the kernel module, run
`cat /proc/count`

You should expect to see the number of currently-running process in your system.

## Cleaning Up

To remove the kernel module from kernel, run
`sudo rmmod proc_count`

## Testing

The module is tested on kernel release version
5.14.8-arch1-1


