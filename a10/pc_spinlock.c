#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"
#include "spinlock.h"

#define MAX_ITEMS 10
const int NUM_ITERATIONS = 200;
const int NUM_CONSUMERS  = 2;
const int NUM_PRODUCERS  = 2;

int producer_wait_count;     // # of times producer had to wait
int consumer_wait_count;     // # of times consumer had to wait
int histogram [MAX_ITEMS+1]; // histogram [i] == # of times list stored i items

int items = 0;

spinlock_t lock;
spinlock_t producer_wait_count_lock;
spinlock_t consumer_wait_count_lock;

void atomic_inc (int* val, spinlock_t* lock) {
  spinlock_lock(lock);
  (*val)++;
  spinlock_unlock(lock);
}


void* producer (void* v) {
  for (int i=0; i<NUM_ITERATIONS; i++) {
    // TODO
    while(1) {
      while (items >= MAX_ITEMS) {
        atomic_inc(&producer_wait_count, &producer_wait_count_lock);
      }
      spinlock_lock(&lock);
      if (items >= MAX_ITEMS) {
        atomic_inc(&producer_wait_count, &producer_wait_count_lock);
        spinlock_unlock(&lock);
      } else {
        break;
      }
    }
    items++;
    histogram[items]++;
    assert (items <= MAX_ITEMS);

    spinlock_unlock(&lock);
  }
  return NULL;
}

void* consumer (void* v) {
  for (int i=0; i<NUM_ITERATIONS; i++) {
    // TODO
    while(1) {
      while (items <= 0) {
        atomic_inc(&consumer_wait_count, &consumer_wait_count_lock);
      }
      spinlock_lock(&lock);
      if (items <= 0) {
        atomic_inc(&consumer_wait_count, &consumer_wait_count_lock);
        spinlock_unlock(&lock);
      } else {
        break;
      }
    }
    items--;
    histogram[items]++;
    assert (items >= 0);

    spinlock_unlock(&lock);

  }
  return NULL;
}

int main (int argc, char** argv) {
  assert(NUM_PRODUCERS == NUM_CONSUMERS);

  uthread_init(NUM_CONSUMERS + NUM_PRODUCERS);
  spinlock_create(&lock);
  spinlock_create(&producer_wait_count_lock);
  spinlock_create(&consumer_wait_count_lock);
  uthread_t threads[NUM_CONSUMERS + NUM_PRODUCERS];

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    threads[i] = uthread_create(producer, 0);
  }

  for (int i = NUM_PRODUCERS; i < (NUM_CONSUMERS + NUM_PRODUCERS); i++) {
    threads[i] = uthread_create(consumer, 0);
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
