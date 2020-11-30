// Justin Gewecke
// This program involves a series of consumers and producers.
// It utilizes semaphores to manage multiple threads
// unlike Monitors, this does not have a control variable.
// References from https://shivammitra.com/reader-writer-problem-in-c/#

#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t block_producer;
int consumerNum = 0;
int producerNum = 0;

#define N 20 // Rotations in the while loops

int produced = 0;
int consumed = 0;

int count = 1;


void *producer(void *id) {
    int c = 0;
	int i = *(int*)id;
	while(c < N){
        pthread_mutex_lock(&mutex);
        producerNum++;
        if (producerNum == 1) {
            sem_wait(&block_producer);
        }
        pthread_mutex_unlock(&mutex);
		//sem_wait(&block_producer);
        produced++;
        printf("Producer %d is producing\n",(*((int *)id)));

        pthread_mutex_lock(&mutex);
        producerNum--;
        if (producerNum == 0) {
            sem_post(&block_producer);
        }
        pthread_mutex_unlock(&mutex);

        //sem_post(&block_producer);
        c++;
	}
}

void *consumer(void *id) {
    int c = 0;
    int i = *(int*)id;
	while(c < N){
        sem_wait(&block_producer);

        consumed++;
        printf("Consumer %d is consuming\n",*((int *)id));
        sem_post(&block_producer);
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