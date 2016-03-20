#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_CYCLES 5120
#define NUMBER_OF_PARALLEL_CYCLES 512
#define NUMBER_OF_LINES 1024
#define NUMBER_OF_LETTERS 10
#define POSITION_OF_NEWLINE 11
#define CHILD_PROCESSES 10
#define CHILD_PROCESSES_READER 3
#define MILLION 1E6
#define NUMBER_OF_THREADS 2
#define BUFFER_SIZE 4

char letterSelector(int position);

char* fileSelector();

char* charSelector();

int validLetter(char letter);
