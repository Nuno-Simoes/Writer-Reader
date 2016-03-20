#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "functions.h"
#include <sys/file.h>
#include <semaphore.h>
#include <string.h>

int svalue;
sem_t sread, sthreads, sbuff;
pthread_mutex_t lock;
char* buffer[BUFFER_SIZE];

void initBuffer() {
	int i;
	
	//pthread_mutex_lock(&lock);
	for(i=0; i<BUFFER_SIZE; i++){
		buffer[i]="empty";
	}
	//pthread_mutex_unlock(&lock);
}

void printBuffer() {
	int i;
	
	//pthread_mutex_lock(&lock);
	for(i=0; i<BUFFER_SIZE; i++) {
		printf("i: %d, buffer[i]: %s\n", i, buffer[i]);
	}
	//pthread_mutex_unlock(&lock);
}

void fillBuffer(char* file){
	
	int i;
	
	//sem_wait(&sread);
	//pthread_mutex_lock(&lock);
	char* copy = (char*)malloc(sizeof(file));
	strcpy(copy, file);
	
	for(i = 0; i < BUFFER_SIZE; i++){
		if(strcmp("empty", buffer[i])==0){
			buffer[i] = copy;
			sem_getvalue(&sbuff, &svalue);
			//if(svalue < BUFFER_SIZE){
				//sem_post(&sthreads);
			//}else{
				//printf("Exceeded buffer size\n");
			//}
			sem_post(&sread);
			//pthread_mutex_unlock(&lock);
			//sem_post(&sread);
			return;
		}
	}
	//pthread_mutex_unlock(&lock);
	//sem_post(&sread);
}

char* getBuffer(){
	
	int i;
	char* result;
	
	//pthread_mutex_lock(&lock);
		
	for(i = 0; i < BUFFER_SIZE; i++){
		if(strcmp("SO2014-0.txt", buffer[i])==0 || strcmp("SO2014-1.txt", buffer[i])==0
			|| strcmp("SO2014-2.txt", buffer[i])==0 || strcmp("SO2014-3.txt", buffer[i])==0
			|| strcmp("SO2014-4.txt", buffer[i])==0){
			result = buffer[i];
			buffer[i] = "empty";
			sem_post(&sbuff);
			pthread_mutex_unlock(&lock);
			return result;
		}
	}
	//pthread_mutex_unlock(&lock);
	return "empty";
}

int reader(){
	
	int i, fd;
	int aux = NUMBER_OF_LINES*POSITION_OF_NEWLINE;
	int lineCounter=0;
	char currentChar;
	char* buf = (char*)malloc(sizeof(char)*(aux+1));
	
	sem_wait(&sthreads);
	sem_post(&sthreads);
	sem_wait(&sread);
	
	char* file = getBuffer();
	
	if((fd = open(file, O_RDONLY)) < 0){
			perror("Cannot open file");
			reader();
			//exit(-1);
	}
	
	if(flock(fd, LOCK_SH) < 0){
			perror("Failed to lock file");
			reader();
			//exit(-1);
		}

	if((read(fd, buf, aux)) < 0){
		perror("Failed to read file");
		reader();
		//exit(-1);
	}
		
	if(validLetter(buf[0])){
		for(i=0; lineCounter < NUMBER_OF_LINES; i++){
			if(i%POSITION_OF_NEWLINE != NUMBER_OF_LETTERS){
				if(buf[i]!=buf[0]) {
					printf("FAILED\n");
					reader();
					//return -1;
				}
			}else{
				if(buf[i] != '\n') {
					printf("FAILED\n");
					//return -1;
					reader();
				}
			lineCounter++;
			}
		}
		
		if(!read(fd, buf, aux)){
			if(flock(fd, LOCK_UN) < 0){
				perror("Failed to unlock file");
				reader();
				//exit(-1);
			}
			if((close(fd))< 0){
				perror("Failed to close file");
				reader();
				//exit(-1);
			}
			printf("SUCCESS, file: %s\n", file);
			reader();
			//return 0;
		}else{
			printf("FAILED\n");
			reader();
			//return -1;
		}
	}else{
		printf("FAILED\n");
		reader();
		//return -1;
	}
}

void* reader_wrapper(void* ptr){
	return(void*) reader((char*) ptr);
}

int main(int argc, char* argv[]){
	
	int i;
	int result[NUMBER_OF_THREADS];
	int stillReading = 1;
	char* input;
	pthread_t tid[NUMBER_OF_THREADS];
	
	//ver condicoes de erro
	sem_init(&sthreads, 0, 0);
	sem_init(&sread, 0,0);
	sem_init(&sbuff, 0, BUFFER_SIZE);
	
	 if(pthread_mutex_init(&lock, NULL) != 0){
        perror("mutex init failed");
        exit(-1);
	}
	
	initBuffer();
	
	for(i = 0; i < NUMBER_OF_THREADS; i++){
		if(pthread_create(&tid[i], NULL, reader_wrapper, NULL) !=0){
			perror("failed to create thread");
			exit(-1);
		}else{
			printf("created thread: %ld\n", tid[i]);
			}
	}
	//erros
	sem_post(&sthreads);
	
	
	while(1){
		
		input = (char*)malloc(sizeof(char)*12);
		scanf("%s", input);
		
		if(strcmp("SO2014-0.txt", input)==0 || strcmp("SO2014-1.txt", input)==0
			|| strcmp("SO2014-2.txt", input)==0 || strcmp("SO2014-3.txt", input)==0
			|| strcmp("SO2014-4.txt", input)==0){
				sem_wait(&sbuff);
				fillBuffer(input);
		}
		
		free(input);
	}
	
	for(i = 0; i < NUMBER_OF_THREADS; i++){
		if(pthread_join(tid[i], (void**)&result[i]) !=0){
			perror("failed to join thread");
			exit(-1);
		}else{
			printf("joined thread: %ld and retuned:%d\n", tid[i], result[i]);
			}
	}
	
	//erros
	pthread_mutex_destroy(&lock);
	
	exit(0);
}


