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
	pid_t forked_pid;
	pid_t* pids = (pid_t*) malloc(argc * sizeof(pid_t));
	int fd[2];
	pipe(fd);
	for (int i = 1; i < argc; i++){
		forked_pid = fork();
		if (forked_pid > 0){
			pids[i - 1] = forked_pid; 
			close(fd[1]);
			if (i > 1){
				close(fd[0]);
			}
			
		}else if (forked_pid < 0){
			printf("child creation failed");
			exit(1);

		}else if (forked_pid == 0 && i == 1){
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			close(fd[0]);
			if (execlp(argv[i], argv[i], NULL) == -1){
				return -1;
			}

		}else{
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			if (execlp(argv[i], argv[i], NULL) == -1){
				return -1;
			}
		}
	}


	
	
	
}	
