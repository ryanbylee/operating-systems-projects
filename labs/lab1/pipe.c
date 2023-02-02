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
				return errno;
		}
	}
	pid_t forked_pid;
	//pid_t* pids = (pid_t*) malloc(argc * sizeof(pid_t));
	int pids[argc - 1];
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
			int status;
			if (waitpid(forked_pid, &status, 0) == -1){
				return errno;
			}
			if (!WIFEXITED(status))
    			return ECHILD;
			else if (WEXITSTATUS(status)){
				return WEXITSTATUS(status);
			}
			
			if (previous_readEnd != STDIN_FILENO){
				close(previous_readEnd); 
			}
			previous_readEnd = fd[0];
			//printf("prev. readEnd after fork(parent): %d\n", previous_readEnd);
			close(fd[1]);

		}

		if (forked_pid == 0){
			//printf("prev. readEnd after fork(child): %d\n", previous_readEnd);
			dup2(previous_readEnd, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
			close(previous_readEnd);
			close(fd[1]);
			close(fd[0]);
			if (execlp(argv[i], argv[i], NULL) == -1){
				return errno;
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
		if (waitpid(forked_pid, &status, 0) == -1){
				return errno;
		}
		if (!WIFEXITED(status))
			return ECHILD;
		else if (WEXITSTATUS(status)){
			return WEXITSTATUS(status);
		}
		
	}
	if (forked_pid == 0){
		//printf("prev. readEnd after fork(child): %d\n", previous_readEnd);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (execlp(argv[argc - 1], argv[argc - 1], NULL) == -1){
				return errno;
		}
	}
	//free(pids);
	/*
	for (int i = 0; i < pidcounter; i++){
			int status;
			waitpid(pids[i], &status,0);
			if (WIFEXITED(status) != 0){
				return WEXITSTATUS(status);
			}
	}
	*/
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
