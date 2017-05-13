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

uthread_mutex_t mx;
uthread_cond_t not_empty;
uthread_cond_t not_full;

void* producer (void* v) {
  for (int i=0; i<NUM_ITERATIONS; i++) {
    // TODO
    uthread_mutex_lock(mx);
    while (items >= MAX_ITEMS) {
      producer_wait_count++;
      uthread_cond_wait(not_full);
    }
    items++;
    histogram[items]++;
    assert(items <= MAX_ITEMS);
    uthread_cond_signal(not_empty);
    uthread_mutex_unlock(mx);
  }
  return NULL;
}

void* consumer (void* v) {
  for (int i=0; i<NUM_ITERATIONS; i++) {
    // TODO
    uthread_mutex_lock(mx);
    while (items <= 0) {
      consumer_wait_count++;
      uthread_cond_wait(not_empty);
    }
    items--;
    histogram[items]++;
    assert(items >= 0);
    uthread_cond_signal(not_full);
    uthread_mutex_unlock(mx);
  }
  return NULL;
}

int main (int argc, char** argv) {
  assert(NUM_PRODUCERS == NUM_CONSUMERS);

  mx = uthread_mutex_create();
  not_empty = uthread_cond_create(mx);
  not_full = uthread_cond_create(mx);
  
  uthread_init(NUM_CONSUMERS + NUM_PRODUCERS);
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
