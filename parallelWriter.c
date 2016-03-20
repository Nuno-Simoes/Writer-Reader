#include <unistd.h>
#include <pthread.h>
#include <signal.h>	
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "functions.h"
#include <sys/file.h>
#include <errno.h>

int activeLock = 1;
int activeErrors = 0;
int stopped = 0;
pthread_mutex_t mutex;

void usr1_handler(int);
void usr2_handler(int);
void tstp_handler(int);

char* induceError(char letter) {
	switch(letter) {
		case 'a':
			return "aaaaaabaaa\n";
		case 'b':
			return "bhbbbbbbbb\n";
		case 'c':
			return "eccccccccc\n";
		case 'd':
			return "dddddbdddd\n";
		case 'e':
			return "eeeeeeeeae\n";
		case 'f':
			return "ffdfffffff\n";
		case 'g':
			return "gggggggigg\n";
		case 'h':
			return "hhhhhghhhh\n";
		case 'i':
			return "iiiijiiiii\n";
		case 'j':
			return "jjajjjjjjj\n";
		default:
			break;
	}
}

int writer(char* nothing){
	
	char* fileName;
	char* valid;
	char* invalid;
	int i, j, k, fd;
	int n= 0;

	while(1){
		
		valid = charSelector();
		invalid = induceError(valid[0]);
		fileName = fileSelector();
		
		if((fd = open(fileName, O_CREAT|O_RDWR, S_IRWXU)) < 0){
			perror("Cannot open file");
			exit(-1);
		}
				
		//pthread_mutex_lock(&mutex);
		if(activeLock) {
			if(flock(fd, LOCK_EX) < 0){
				perror("Failed to lock file");
				exit(-1);
			}
		}
		//pthread_mutex_unlock(&mutex);
	
		for(j = 0; j<NUMBER_OF_LINES; j++){
			
			//pthread_mutex_lock(&mutex);
			if(activeErrors && (j%2)==0) {
				//pthread_mutex_unlock(&mutex);
				if(write(fd, invalid, strlen(invalid)) < 0){
					perror("Failed to write file");
					//writer();
				}
			} else {
				if(write(fd, valid, strlen(valid)) < 0){
					perror("Failed to write file");
					//writer();
				}
			}
		}
		
		//pthread_mutex_lock(&mutex);
		if(activeLock) {
			if(flock(fd, LOCK_UN) < 0){
				perror("Failed to unlock file");
				exit(-1);
			}
		}
		//pthread_mutex_unlock(&mutex);
					
		if(close(fd) < 0){
			perror("Failed to close file");
			exit(-1);
		}
		
		//fclose(pfile);
		
		//pthread_mutex_lock(&mutex);
		if(stopped) {
			//pthread_mutex_unlock(&mutex);
			return 0;
		}
		//pthread_mutex_unlock(&mutex);
		
	}
	return 0;
}

void* writer_wrapper(void* ptr){
	return(void*) writer((char*) ptr);
}


int main(int argc, char* argv[]){
	int i;
	int result[NUMBER_OF_THREADS];
	pthread_t tid[NUMBER_OF_THREADS];
		
	if(pthread_mutex_init(&mutex, NULL) != 0){
        perror("mutex init failed");
        exit(-1);
	}	
	struct sigaction usr1_action;
	struct sigaction usr2_action;
	struct sigaction tstp_action;
    
    /* prepara a estrutura que descreve a nova acção. */
    usr1_action.sa_handler = usr1_handler;
    sigemptyset (&usr1_action.sa_mask);
    sigaddset(&usr1_action.sa_mask, SIGUSR1);
    usr1_action.sa_flags = 0;
    sigaction(SIGUSR1, &usr1_action, NULL); /* associa acção ao signal */
    
    /* prepara a estrutura que descreve a nova acção. */
    usr2_action.sa_handler = usr2_handler;
    sigemptyset (&usr2_action.sa_mask);
    sigaddset(&usr2_action.sa_mask, SIGUSR2);
    usr2_action.sa_flags = 0;
    sigaction(SIGUSR2, &usr2_action, NULL); /* associa acção ao signal */
    
    /* prepara a estrutura que descreve a nova acção. */
    tstp_action.sa_handler = tstp_handler;
    sigemptyset (&tstp_action.sa_mask);
    sigaddset(&tstp_action.sa_mask, SIGTSTP);
    tstp_action.sa_flags = 0;
    sigaction(SIGTSTP, &tstp_action, NULL); /* associa acção ao signal */
	
	for(i = 0; i < NUMBER_OF_THREADS; i++){
		if(pthread_create(&tid[i], NULL, writer_wrapper, NULL) !=0){
			perror("failed to create thread");
			exit(-1);
		}else{
			printf("created thread: %ld\n", tid[i]);
		}
	}
	
	for(i = 0; i < NUMBER_OF_THREADS; i++){
		if(pthread_join(tid[i], (void**)&result[i]) !=0){
			perror("failed to join thread");
			exit(-1);
		}else{
			printf("joined thread: %ld and returned:%d\n", tid[i], result[i]);
		}
	}
	
	pthread_mutex_destroy(&mutex);
	exit(0);
}

void usr1_handler(int signum)
{
	if(activeLock) {
		activeLock = 0;
	} else
		activeLock = 1;
} 

void usr2_handler(int signum)
{
	if(activeErrors) {
		activeErrors = 1;
	} else
		activeErrors = 0;
} 

void tstp_handler(int signum)
{
	printf("TSTP\n");
	stopped = 1;
} 
