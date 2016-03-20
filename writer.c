#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "functions.h"
#include <sys/file.h>
#include <errno.h>

int main(int argc, char *argv[]){
	
	char* buf;
	int i, j, fd;

	for(i=0; i<NUMBER_OF_PARALLEL_CYCLES; i++){
	
		buf = charSelector();
		
		if((fd = open(fileSelector(), O_CREAT|O_RDWR, S_IRWXU)) < 0){
			perror("Cannot open file");
			exit(-1);
		}
		
		if(flock(fd, LOCK_EX) < 0){
			perror("Failed to lock file");
			exit(-1);
		}
	
		for(j = 0; j<NUMBER_OF_LINES; j++){
			if(write(fd, buf, strlen(buf)) < 0){
				perror("Failed to write file");
				exit(-1);
			}
		}
		
		if(flock(fd, LOCK_UN) < 0){
			perror("Failed to unlock file");
			exit(-1);
		}
					
		if(close(fd) < 0){
			perror("Failed to close file");
			exit(-1);
		}
	}
	return 0;
}
