# You Spin Me Round Robin
C program that calculates the average waiting time/response time of the given processes with arrival time/burst time with the Round Robin Scheduler.
## Building
On unix/linux command-line interface, type 

`make`

to create the executable
## Running
Run the executable with two arguments: .txt file that lists all processes and their arrival/burst time, as well as the quantum length.

e.g. `./rr processes.txt 3`
## Cleaning up
To clean up the binary files, type

`make clean`