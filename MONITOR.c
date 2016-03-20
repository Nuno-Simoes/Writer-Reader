#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "functions.h"
#include <signal.h>

int main(int argc, char *argv[]){

	pid_t pids[2];
	pid_t pid1, pid2;
	int i, status;
	int n = 2;
	int fd[2];
	char* input;
	
	pipe(fd);
	pid1 = fork();
	
	if(pid1 == 0) {
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		printf("HELLLLOOO FIlho2: %d\n", getpid());
		execlp("./LEITOR-MONITOR", "LEITOR-MONITOR", NULL);
	} else {
		pid2 = fork();
		
		if(pid2 == 0) {
			close(fd[0]);
			close(fd[1]);
			printf("HELLLLOOO FIlho: %d\n", getpid());
			execlp("./parallelWriter", "parallelWriter", NULL);
		} else {
			close(fd[0]);
			while(1) {
				
				input = (char*)malloc(sizeof(char)*12);
				scanf("%s", input);
				
				if(strcmp("il", input)==0){
					kill(pid2, SIGUSR1);
				}else if(strcmp("ie", input)==0){
					kill(pid2, SIGUSR2);
				}else if(strcmp("sair", input)==0){
					close(fd[1]);
					kill(pid2, SIGTSTP);
					break;
				} else
					write(fd[1], input, strlen(input));
			}
		}
	}
	
	while(n > 0){
		pid1 = wait(&status);
		pid2 = wait(&status);
		
		if(pid1 == -1){
			printf("%d\n", getpid());
			perror("wait failed");
			exit(-1);
		}
		
		if(pid2 == -1){
			printf("%d\n", getpid());
			perror("wait failed");
			exit(-1);
		}
	
		n--;
	}			
	exit(0);
}
