#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "functions.h"
#include <sys/file.h>

int main(int argc, char *argv[]){
	
	int i, fd;
	int aux = NUMBER_OF_LINES*POSITION_OF_NEWLINE;
	int lineCounter=0;
	char currentChar;
	char* buf = (char*)malloc(sizeof(char)*(aux+1));
	char* file;
	
	printf("%s\n", argv[1]);
	
	if(argv[1] == NULL){
		file = fileSelector();
	}else{
		file = argv[1];
	}
	
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
					return -1;
				}
			}else{
				if(buf[i] != '\n') {
					return -1;
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
			return 0;
		}else{
			return -1;
		}
	}else{
		return -1;
	}
}
