// Justin Gewecke
// My solution is comprised of the code within the get() and put() functions.
// Using a combination of locks and pthread signals and waits, I can make the
// different threads interact with each other without them over stepping their
// boundaries. Consumer can't consume past 0 and Producers can't produce past
// 100.

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t Buffer_Not_Full = PTHREAD_COND_INITIALIZER;
static pthread_cond_t Buffer_Not_Empty = PTHREAD_COND_INITIALIZER;

#define N 100
int count = 0;
int *buffer;
int theProduct;

// Consumer function
int get() {
    pthread_mutex_lock(&mutex); // Lock
    if (count > -1) pthread_cond_wait(&Buffer_Not_Empty, &mutex); // Sleep
    count = count - 1;
    pthread_mutex_unlock(&mutex); // Unlock
    pthread_cond_signal(&Buffer_Not_Full); // wakeup

    return count;
}

// Producer function
void put(int number) {
    pthread_mutex_lock(&mutex); // Lock
    if (count == N) pthread_cond_wait(&Buffer_Not_Full, &mutex); // Sleep
    count = count + 1;
    buffer[count] = number;
    printf("%d",count);
    pthread_mutex_unlock(&mutex); // Unlock
    pthread_cond_signal(&Buffer_Not_Empty); // WakeUp
}

int produce(){
	return theProduct++;
}

void consume(int i){
	printf("%i, ", i);
}


void *producer(void *arg) {
	int i;
	while(1){
		i = produce();
		put(i);
	}
}

void *consumer(void *arg) {
	int i;
	while(1){
		i = get();
		consume(i);
	}
}

int main() {
    buffer = (int *) malloc(sizeof(int) * N); // Create buffer

    pthread_t producer_t;
    pthread_t consumer_t;

    pthread_create(&producer_t, NULL, producer, NULL);
    pthread_create(&consumer_t, NULL, consumer, NULL);

    pthread_join(producer_t, NULL);
    pthread_join(consumer_t, NULL);

    return 0;
}
