#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "functions.h"
#include <sys/file.h>

void* reader(void *result){
	
	int i, fd;
	int aux = NUMBER_OF_LINES*POSITION_OF_NEWLINE;
	int lineCounter=0;
	char currentChar;
	char* buf = (char*)malloc(sizeof(char)*(aux+1));
	char* file = fileSelector();
			
	if((fd = open(file, O_RDONLY)) < 0){
			perror("Cannot open file");
			exit(-1);
	}
	
	if(flock(fd, LOCK_SH) < 0){
			perror("Failed to lock file");
			exit(-1);
		}

	if((read(fd, buf, aux)) < 0){
		perror("Failed to read file");
		exit(-1);
	}
		
	if(validLetter(buf[0])){
		for(i=0; lineCounter < NUMBER_OF_LINES; i++){
			if(i%POSITION_OF_NEWLINE != NUMBER_OF_LETTERS){
				if(buf[i]!=buf[0]) {
					*((int*)result) = -1;
					pthread_exit(result);
				}
			}else{
				if(buf[i] != '\n') {
					*((int*)result) = -1;
					pthread_exit(result);
				}
			lineCounter++;
			}
		}
		
		if(!read(fd, buf, aux)){
			if(flock(fd, LOCK_UN) < 0){
				perror("Failed to unlock file");
				exit(-1);
			}
			if((close(fd))< 0){
				perror("Failed to close file");
				exit(-1);
			}
			*((int*)result) = 0;
			pthread_exit(result);
		}else{
			*((int*)result) = -1;
			pthread_exit(result);
		}
	}else{
		*((int*)result) = -1;
		pthread_exit(result);
	}
}


int main(int argc, char *argv[]){
	
	int i;
	int result[NUMBER_OF_THREADS];
	pthread_t tid[NUMBER_OF_THREADS];
	void* status;
	srand(time(NULL));	
	
	for(i = 0; i < NUMBER_OF_THREADS; i++){
		if(pthread_create(&tid[i], NULL, reader, &result[i]) != 0){
			perror("Failed to create thread");
			exit(-1);
		}else
			printf("Foi criada a tarefa %d\n", (int)tid[i]);
	}
	
	for(i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_join(tid[i], &status);
		printf("A tarefa %d terminou e devolveu:%d\n", (int)tid[i], result[i]);
	}
	
	exit(0);
}
