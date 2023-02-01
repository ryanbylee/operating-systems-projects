#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/*
notes:
for every command before the last:
	set up file descriptors with open, dup3, and close
	fork
	exec with appropriate command



*/
int main(int argc, char *argv[])
{
	if (argc == 1){
		return EINVAL;
	}

	if (argc == 2){
		if (execlp(argv[1], argv[1], NULL) == -1){
				return -1;
		}
	}
	pid_t forked_pid;
	pid_t* pids = (pid_t*) malloc(argc * sizeof(pid_t));
	int pidcounter = 0;
	int fd[2];
	int previous_readEnd = STDIN_FILENO;
	for (int i = 1; i < argc - 1; i++){
		pipe(fd);
		//printf("fd[0]: %d\n", fd[0]);
		//printf("fd[1]: %d\n", fd[1]);
		//printf("prev. readEnd: %d\n", previous_readEnd);
		forked_pid = fork();
		if (forked_pid > 0){
			pids[i - 1] = forked_pid; 
			previous_readEnd = fd[0];
			//printf("prev. readEnd after fork(parent): %d\n", previous_readEnd);
			close(fd[1]);
		}

		if (forked_pid == 0){
			//printf("prev. readEnd after fork(child): %d\n", previous_readEnd);
			dup2(previous_readEnd, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			close(fd[0]);
			if (execlp(argv[i], argv[i], NULL) == -1){
				return -1;
			}
		}
	}

	//last command
	forked_pid = fork();
	if (forked_pid > 0){
		pids[argc - 2] = forked_pid; 
		previous_readEnd = fd[0];
		//printf("prev. readEnd after fork(parent): %d\n", previous_readEnd);
		close(fd[1]);
		int status;
		for (int i = 0; i < pidcounter; i++){
			waitpid(pids[i], &status,0);
			if (WEXITSTATUS(status) != 0){
				printf("%d\n", WEXITSTATUS(status));
			}
		}
	}

	if (forked_pid == 0){
		//printf("prev. readEnd after fork(child): %d\n", previous_readEnd);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (execlp(argv[argc - 1], argv[argc - 1], NULL) == -1){
				return -1;
		}
	}
}	

/*
	int previous_readEnd = STDIN_FILENO;
	for (int i = 1; i < argc; i++){
		
		forked_pid = fork();
		if (forked_pid > 0){
			pids[i - 1] = forked_pid; 
			close(fd[1]);
			previous_readEnd = fd[0];
			if (i < argc - 1){
				pipe(fd);
			}
			

		}else if (forked_pid < 0){
			printf("child creation failed");
			exit(1);

		}else if (forked_pid == 0 && i == 1){	//first command
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			
			close(fd[0]);
			
			if (execlp(argv[i], argv[i], NULL) == -1){
				return -1;
			}

		}else if (forked_pid == 0 && i == argc - 1){	//last command
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			if (execlp(argv[i], argv[i], NULL) == -1){
				return -1;
			}
		}else{	//middle commands
			dup2(previous_readEnd, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			close(fd[0]);
			if (execlp(argv[i], argv[i], NULL) == -1){
				return -1;
			}
		}
	}
	
	*/
