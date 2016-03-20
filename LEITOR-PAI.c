#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "functions.h"

int main(int argc, char *argv[]){

	pid_t pids[CHILD_PROCESSES_READER];
	pid_t pid;
	int i, status, valorExit;
	int n = CHILD_PROCESSES_READER;
	int r[CHILD_PROCESSES_READER];
	int j = 0;
	
	for(i = 0; i < n; i++){
		if((pids[i] = fork()) < 0){
			perror("fork() failed\n");
			abort();
		}else 
			if(pids[i] == 0){
				srand(time(NULL)*i);
				execl("./reader", "reader", fileSelector(), NULL);
				perror("Failed exec\n");
				exit(-1);
			}
	}
	
	while(i > 0){
		pid = wait(&status);
		
		if(pid == -1){
			perror("wait failed");
			exit(-1);
		}
		
		if(WIFEXITED(status)){
			valorExit = (char)WEXITSTATUS(status);
			r[j]=valorExit;
			j++;
		}else
			printf("Child with pid=%ld completed without exit/return\n", (long)pid); 

		i--;
	}		

	for(i = 0; i < CHILD_PROCESSES_READER; i++)
		printf("Reader %d:%d\n", i+1, r[i]);
}
