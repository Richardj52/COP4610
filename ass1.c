#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#define NUM_THREADS			200 		


int SharedVariable = 0;

void * SimpleThread(void *which){
	long which2 = (long)which;
	int num;
	int val;

	for(num=0; num < 20; num++){
		if(random() > RAND_MAX/2)
		usleep(500);

		val = SharedVariable;
		printf("**** thread %ld sees value %d\n",which2,val);
		SharedVariable = val+1;
	}
		val = SharedVariable;
		printf("Thread %ld sees final value %d\n", which2,val);
		pthread_exit(NULL);
}

int main(int argc, char **argv[]){

	pthread_t threads[NUM_THREADS];	
	int rc;
	long i;
	for(i = 0; i < NUM_THREADS; i++){
		printf("This is our %ld thread\n",i);
		rc = pthread_create(&threads[i], NULL, SimpleThread, (void *) i);
	if(rc){
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);

		}

	}
	pthread_exit(NULL);

}
