#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define PTHREAD_SYNC 1	

int SharedVariable = 0;
#if PTHREAD_SYNC
//intializeing of mutex to sync access to SharedVariable
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//intialization of barrier for threads to wait
pthread_barrier_t barrier;
#endif

void* SimpleThread(void *args){
	long which = (long)args;
	int num, val;
	
	for(num = 0; num < 20; num++){
		if(random() > RAND_MAX/2){
			usleep(500);
		}
	//locking SharedVariable from being access at the same time
	#if PTHREAD_SYNC
		pthread_mutex_lock(&mutex);
	#endif
		
		val = SharedVariable;
		printf("*** thread %ld sees value %d\n", which, val);
		SharedVariable = val + 1;

	//other way around (unlocking SharedVariable to be access)
	#if PTHREAD_SYNC
		pthread_mutex_unlock(&mutex);
	#endif
	}
	//Once threads has finished final value of SharedVariable is shown
	#if PTHREAD_SYNC
		pthread_barrier_wait(&barrier);
	#endif
	val = SharedVariable;
	printf("Thread %ld sees value %d\n", which, val);
	pthread_exit(0);
}
int main(int argc, char **argv){
	//Validating the arguments passed when running file
	if(argc != 2){
		printf("Wrong amount of arguments\n");
		exit(-1);
	}
	else{
	//barrier being instialized
	#if PTHREAD_SYNC
		pthread_barrier_init(&barrier,NULL,atoi(argv[1])+1);
	#endif
		pthread_t threads[atoi(argv[1])];
	//pthread_create is going to create a thread depending on user input
		for(long i=0; i < atoi(argv[1]); i++){
			printf("In main: creating thread %ld\n", i);
			pthread_create(&threads[i], NULL, SimpleThread, (void *)i);
		}
	
	#if PTHREAD_SYNC
		pthread_barrier_wait(&barrier);
	#endif
	for(int i = 0; i < atoi(argv[1]); i++){
		pthread_join(threads[i],NULL);
	}
		pthread_exit(0);
	}

}
