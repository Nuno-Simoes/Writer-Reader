#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "functions.h"
#include <sys/file.h>
#include <stddef.h>

char* file, letter;
int k, result, lastThread = 0;
pthread_t tid[NUMBER_OF_THREADS];
int threadid=0;

void* reader(void *nothing){
	
	int i, fd;
	int aux = NUMBER_OF_LINES*POSITION_OF_NEWLINE;
	int lineCounter=0;
	char currentChar;
	char* buf = (char*)malloc(sizeof(char)*(aux+1));
	
	if((fd = open(file, O_RDONLY)) < 0){
		perror("Cannot open file");
		exit(-1);
	}
	
	if(flock(fd, LOCK_SH) < 0){
		perror("Failed to lock file");
		exit(-1);
	}
	
	lseek(fd, (threadid++)*(NUMBER_OF_LINES/k)*POSITION_OF_NEWLINE, SEEK_SET);
	
	if((read(fd, buf, aux)) < 0){
		perror("Failed to read file");
		exit(-1);
	}
		
	if(validLetter(letter)){
		int numberOfLines;
		
		if (lastThread==1)
			numberOfLines = NUMBER_OF_LINES/k + NUMBER_OF_LINES%k;
		else
			numberOfLines = NUMBER_OF_LINES/k;
		
		for(i=0; lineCounter < numberOfLines; i++){
			if(i%POSITION_OF_NEWLINE != NUMBER_OF_LETTERS){
				if(buf[i]!=letter) {
					result = -1;
					pthread_exit(0);
				}
			}else{
				if(buf[i] != '\n') {
					result = -1;
					pthread_exit(0);
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
			result = 0;
			pthread_exit(0);
		}else if (lastThread == 1){
			result = -1;
			pthread_exit(0);
		}
	}else{
		result = -1;
		pthread_exit(0);
	}
}

int main(int argc, char *argv[]){
	
	int i, fd;
	int aux = NUMBER_OF_LINES*POSITION_OF_NEWLINE;
	void* status;
	char* buf = (char*)malloc(sizeof(char)*(aux+1));
	
	srand(time(NULL));	
	file = fileSelector();	
	
	if (argv[1]==NULL) {
		printf("Insert value for K\n");
		return 0;
	} else
		k = strtol(argv[1], NULL, 10);
	
	if((fd = open(file, O_RDONLY)) < 0){
		perror("Cannot open file");
		exit(-1);
	}
	
	(read(fd, buf, aux));
	letter = buf[0];
	
	if((close(fd))< 0){
		perror("Failed to close file");
		exit(-1);
	}
	
	if (k > NUMBER_OF_LINES){
		printf("Invalid K\n");
		exit(-1);
	}
				
	for(i = 0; i < k; i++){
		
		if (i==k-1)
			lastThread=1;
			
		if(pthread_create(&tid[i], NULL, reader, &i) != 0){
			perror("Failed to create thread");
			exit(-1);
		} else
			printf("Foi criada a tarefa %d\n", (int)tid[i]);
			
		if (result==-1)
			break;
	}
	
	printf("Resultado: %d\n", result);
	
	exit(0);
}
