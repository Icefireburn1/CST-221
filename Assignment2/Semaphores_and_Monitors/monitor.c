// Justin Gewecke
// This program involves a series of consumers and producers.
// It utilizes a monitor via a mutual exclusion lock (Mutexes)
// and a condition variable (pthread_cond_t)
// References from https://www.geeksforgeeks.org/reader-writer-problem-using-monitors-pthreads/?ref=rp

#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t canproduce = PTHREAD_COND_INITIALIZER;
static pthread_cond_t canconsume = PTHREAD_COND_INITIALIZER;

#define N 20 // Rotations in the while loops

int activeConsumers = 0;
int activeProducers = 0;

int waitingProducers = 0;
int waitingConsumers = 0;

int produced = 0;
int consumed = 0;

void BeginProduce(int i)
{
    pthread_mutex_lock(&mutex); // Lock

    // Enter if no active or waiting readers/writers
    if (activeProducers == 1 || activeConsumers > 0)
    {
        ++waitingProducers;
        pthread_cond_wait(&canproduce, &mutex);
        --waitingProducers;
    }

    activeProducers = 1;
    printf("producer %d is producing\n", i);
    produced++;
    pthread_mutex_unlock(&mutex); // Unlock
}

void EndProduce(int i)
{
    pthread_mutex_lock(&mutex); // Lock
    activeProducers = 0;

    // Check if any consumerss are waiting
    if (waitingConsumers > 0)
    {
        pthread_cond_signal(&canconsume);
    }
    else
    {
        pthread_cond_signal(&canproduce);
    }
    pthread_mutex_unlock(&mutex);
}

void BeginConsume(int i)
{
    pthread_mutex_lock(&mutex);

    // Consumer can enter if there are no writers active or waiting for
    if (activeProducers == 1 || waitingProducers > 0)
    {
        waitingConsumers++;

        // consumer waits
        pthread_cond_wait(&canconsume, &mutex);
        waitingConsumers--;
    }

    activeConsumers++;
    printf("consumer %d is consuming\n", i);
    consumed++;
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&canconsume);
}

void EndConsume(int i)
{
    pthread_mutex_lock(&mutex);

    // When reader finishes, if it is the last reader
    // it lets the producer in
    if (--activeConsumers == 0)
    {
        pthread_cond_signal(&canproduce);
    }

    pthread_mutex_unlock(&mutex);
}


void *producer(void *id) {
    int c = 0;
	int i = *(int*)id;
	while(c < N){
		BeginProduce(i);
        EndProduce(i);
        c++;
	}
}

void *consumer(void *id) {
    int c = 0;
    int i = *(int*)id;
	while(c < N){
		BeginConsume(i);
        EndConsume(i);
        c++;
	}
}

int main() {
    pthread_t p[5], c[5]; // 5 Readers and 5 Writers
    int id[5]; // Ids for our threads

    // Create threads and provide ids to them
    for (int i = 0; i < 5; i++) { 
        id[i] = i; 
  
        // creating threads which execute reader function 
        pthread_create(&c[i], NULL, &consumer, &id[i]); 
  
        // creating threads which execute writer function 
        pthread_create(&p[i], NULL, &producer, &id[i]); 
    } 
  
    // Make sure we wait for our threads before finishing the program
    for (int i = 0; i < 5; i++) { 
        pthread_join(c[i], NULL); 
    } 
    for (int i = 0; i < 5; i++) { 
        pthread_join(p[i], NULL); 
    } 

    printf("Produced %d\n", produced);
    printf("Consumed %d\n", consumed);

    return 0;
}