x#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_sem.h"

#define MAX_ITEMS 10
const int NUM_ITERATIONS = 200;
const int NUM_CONSUMERS  = 2;
const int NUM_PRODUCERS  = 2;

int producer_wait_count;     // # of times producer had to wait
int consumer_wait_count;     // # of times consumer had to wait
int histogram [MAX_ITEMS+1]; // histogram [i] == # of times list stored i items

struct People {
  uthread_sem_t lock;
  uthread_sem_t isNotEmpty;
  uthread_sem_t isNotFull;
  int items;
};

struct People* createPeople() {
  struct People* people = malloc (sizeof (struct People));
  people->lock = uthread_sem_create(1);
  people->isNotEmpty = uthread_sem_create(0);
  people->isNotFull = uthread_sem_create(MAX_ITEMS);
  people->items = 0;
  return people;
}

void* producer (void* v) {
  struct People* people = v;

  for (int i=0; i<NUM_ITERATIONS; i++) {
    // TODO
    producer_wait_count++;
    uthread_sem_wait(people->isNotFull);
    uthread_sem_wait(people->lock);
    people->items++;
    histogram[people->items] += 1;
    assert(people->items <= MAX_ITEMS);
    uthread_sem_signal(people->isNotEmpty);
    uthread_sem_signal(people->lock);
  }
  return NULL;
}

void* consumer (void* v) {
  struct People* people = v;
  for (int i=0; i<NUM_ITERATIONS; i++) {
    // TODO
    consumer_wait_count++;
    uthread_sem_wait(people->isNotEmpty);
    uthread_sem_wait(people->lock);
    assert(people->items > 0);
    people->items--;
    histogram[people->items] += 1;
    uthread_sem_signal(people->isNotFull);
    uthread_sem_signal(people->lock);
  }
  return NULL;
}

int main (int argc, char** argv) {
  assert(NUM_PRODUCERS == NUM_CONSUMERS);

  uthread_t threads[NUM_CONSUMERS + NUM_PRODUCERS];

  uthread_init(NUM_CONSUMERS + NUM_PRODUCERS);
  

  struct People* people = createPeople();

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    threads[i] = uthread_create(producer, people);
  }

  for (int i = NUM_PRODUCERS; i < (NUM_CONSUMERS + NUM_PRODUCERS); i++) {
    threads[i] = uthread_create(consumer, people);
  }

  for (int i = 0; i < (NUM_CONSUMERS + NUM_PRODUCERS); i++) {
    uthread_join(threads[i], 0);
  }

  // TODO: Create Threads and Join

  printf ("producer_wait_count=%d, consumer_wait_count=%d\n", producer_wait_count, consumer_wait_count);
  printf ("items value histogram:\n");
  int sum=0;
  for (int i = 0; i <= MAX_ITEMS; i++) {
    printf ("  items=%d, %d times\n", i, histogram [i]);
    sum += histogram [i];
  }
  assert (sum == sizeof (threads) / sizeof (uthread_t) * NUM_ITERATIONS);
}
