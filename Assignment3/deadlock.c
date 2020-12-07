// This solution uses semaphores, mutexes, 2 processes,
// and 10 threads to practice preventing deadlock
// By Justin Gewecke

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
 
#define N 10
#define FILE_NAME "lorem.txt"
static pthread_mutex_t _mutex1 = PTHREAD_MUTEX_INITIALIZER;

sem_t _sem;
	
void *ReadFile(void *args)
{
	pthread_mutex_lock(&_mutex1);

	FILE *fp;
	fp = fopen(FILE_NAME, "w");
    	
    	int id = *(int*)args;
	
    	sleep(3);
   	if (fp == NULL) {
   	    printf("Thread %d is waiting...", id);
   	    sem_wait(&_sem);
  	}
  	char thread_number = id + '0';
  	fputs("Hello from thread ", fp);
   	fprintf(fp, "%d", id);
   	fputs("!\n",fp);

  	printf("Thread %d finished writing itself!\n", id);

  	sem_post(&_sem);
  	
  	
  	fclose(fp);
  	pthread_mutex_unlock(&_mutex1);
}

void ExecuteThreads()
{
    sem_init(&_sem, 0, 1);
    int id[N];
    pthread_t my_threads[N]; // My threads


    for (int i = 0; i < N; i++) {
        id[i] = i;
        // Create threads
        pthread_create(&my_threads[i], NULL, ReadFile, &id[i]);
    }
    
    for (int i = 0; i < N; i++) {
    	// Join threads 
        pthread_join(my_threads[i], NULL);
    }

    printf("Finished\n");
}
 
int main()
{   
    // Can't figure out how to share memory
    int i = fork();
    if (i == 0) // child
    {
        ExecuteThreads();
    }
    else // parent
    {
        ExecuteThreads();
    }

    return 0;
}