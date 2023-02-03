#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	//no command
	if (argc == 1){
		return EINVAL;
	}

	//only one command
	if (argc == 2){
		if (execlp(argv[1], argv[1], NULL) == -1){
				return errno;
		}
	}

	//first to last - 1 commands
	pid_t forked_pid;
	int fd[2];
	int previous_readEnd = STDIN_FILENO;
	for (int i = 1; i < argc - 1; i++){
		pipe(fd);
		forked_pid = fork();
		//parent process
		if (forked_pid > 0){
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
			close(fd[1]);

		}
		//child process
		if (forked_pid == 0){
			dup2(previous_readEnd, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			close(fd[0]);
			if (execlp(argv[i], argv[i], NULL) == -1){
				return errno;
			}
			close(previous_readEnd);
		}
	}

	//last command
	forked_pid = fork();
	//parent process
	if (forked_pid > 0){
		previous_readEnd = fd[0];
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
	//child process
	if (forked_pid == 0){
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (execlp(argv[argc - 1], argv[argc - 1], NULL) == -1){
				return errno;
		}
	}
}