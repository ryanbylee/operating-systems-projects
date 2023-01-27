#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>

/*
notes:
for every command before the last:
	set up file descriptors with open, dup3, and close
	fork
	exec with appropriate command



*/
int main(int argc, char *argv[])
{

	pid_t child_pid;
	pid_t* pids = (pid_t*) malloc(argc * sizeof(pid_t));
	for (int i = 1; i < argc + 1; i++){
		child_pid = fork();
		if (child_pid > 0){
			pids[i - 1] = child_pid;
		}
		
	}
	if (child_pid > 0){
		for(int i = 0; i < 2; i++){
			printf("%d\n", pids[i]);
		}
		printf("parent's pid is: %d\n", getpid());
	}
	
	
}	
