#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argv[]){


	srand(time(NULL));

	if(argc!=2){
		printf("Usage:%s <number of randoms>\n",argv[0]);
		exit(1);
	}

	int number = atoi(argv[1]);

	printf("DW ");
	for(int i=0;i<number;++i){
		printf("0%02xH, ",rand()%255);
	}


	return 0;
}