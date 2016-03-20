#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

char* stringArray[10] = {"aaaaaaaaaa\n", "bbbbbbbbbb\n", "cccccccccc\n",	
"dddddddddd\n", "eeeeeeeeee\n", "ffffffffff\n", "gggggggggg\n",
"hhhhhhhhhh\n", "iiiiiiiiii\n", "jjjjjjjjjj\n"};

char* fileArray[5] = {"SO2014-0.txt", "SO2014-1.txt", "SO2014-2.txt",
"SO2014-3.txt", "SO2014-4.txt"};

char letterArray[10]= {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

struct thread_param {
	char *file;
	int value;
};	

char letterSelector(int position){
	return letterArray[position];
}	

char* fileSelector(){
		
	int number = rand()%5;
	
	return fileArray[number];
}

//char* fileSelector(int numberOfFiles){
	
	//int size = sizeof(fileArray);
	//int numbers[size];
	//int i, n, tmp;
	//char** resultFileArray[numberOfFiles];
		
	//if(numberOfFiles > size){
		//printf("Invalid number of files\n");
		//exit(-1);
	//}
	
	//for(i = 0; i < size; i++)
		//numbers[i] = i;
	
	//for(i = size; i > 1;){
		//n = rand()%i;
		//tmp = numbers[n];
		//i--;
		//numbers[n] = numbers[i];
		//numbers[i] = tmp;
	//}
	
	//for(i = 0; i < numberOfFiles; i++)
		//resultFileArray[i] = fileArray[numbers[i]];
	
	//return resultFileArray;
//}

char* charSelector(){
		
	int number = rand()%10;
	
	return stringArray[number];

}

int validLetter(char letter){
	
	int i;
	
	for(i = 0; i<sizeof(letterArray); i++){
		if(letter == letterArray[i])
			return 1;
	}
	return 0;
}	
