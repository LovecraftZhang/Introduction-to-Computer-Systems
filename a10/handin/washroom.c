#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

/**
 * You might find these declarations useful.
 */
enum GenderIdentity {MALE = 0, FEMALE = 1};
const static enum GenderIdentity otherGender [] = {FEMALE, MALE};

struct Washroom {
  // TODO
  uthread_mutex_t mutex;
  uthread_cond_t male;
  uthread_cond_t female;
  int sum;
  int entered;
  enum GenderIdentity g;
};

struct Washroom* createWashroom() {
  struct Washroom* washroom = malloc (sizeof (struct Washroom));
  // TODO
  washroom->mutex = uthread_mutex_create();
  washroom->male = uthread_cond_create(washroom->mutex);
  washroom->female = uthread_cond_create(washroom->mutex);
  washroom->sum = 0;
  washroom->entered = 0;
  washroom->g = 0;
  return washroom;
}

struct Washroom* washroom;

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogrammutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void signalGender (struct Washroom* washroom, enum GenderIdentity g) {
  for (int i = 0; i < 3; i++) {
    if (g == MALE) {
      uthread_cond_signal(washroom->male);
    } else if (g == FEMALE) {
      uthread_cond_signal(washroom->female);
    }
  }
}

void waitGender (struct Washroom* washroom, enum GenderIdentity g) {
  if (g == MALE) {
    uthread_cond_wait(washroom->male);
  } else if (g == FEMALE) {
    uthread_cond_wait(washroom->female);
  }
}

void enterWashroom (struct Washroom* washroom, enum GenderIdentity g) {
  // TODO
  uthread_mutex_lock(washroom->mutex);
  if (washroom->sum == 0) {
    washroom->g = g;
  } else {
    int e = washroom->entered;
    waitGender(washroom, g);
    int waitingTime = washroom->entered - e;
    if (waitingTime < WAITING_HISTOGRAM_SIZE) {
      waitingHistogram[waitingTime]++;
    } else {
      waitingHistogramOverflow++;
    }
  }
  washroom->entered++;
  washroom->sum++;
  occupancyHistogram[g][washroom->sum]++;
  uthread_mutex_unlock(washroom->mutex);
}

void leaveWashroom(struct Washroom* washroom) {
  // TODO
  uthread_mutex_lock(washroom->mutex);
  washroom->sum--;
  occupancyHistogram[washroom->g][washroom->sum]++;
  if (washroom->sum == 0) {
    washroom->g = otherGender[washroom->g];
    signalGender(washroom, washroom->g);
  }
  uthread_mutex_unlock(washroom->mutex);
}

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogrammutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogrammutex);
}

void* users (void* av) {
  struct Washroom* washroom = av;
  enum GenderIdentity g = random() % 2;
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    enterWashroom(washroom, g);
    for (int i = 0; i < NUM_PEOPLE; i++) {
      uthread_yield();
    }
    leaveWashroom(washroom);
    for (int i = 0; i < NUM_PEOPLE; i++) {
      uthread_yield();
    }
  }
}

//
// TODO
// You will probably need to create some additional produres etc.
//

int main (int argc, char** argv) {
  uthread_init (1);
  washroom = createWashroom();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogrammutex = uthread_mutex_create ();

  // TODO

  for (int i = 0; i < NUM_PEOPLE; i++) {
    pt[i] = uthread_create(users, washroom);
  }
  for (int i = 0; i < NUM_PEOPLE; i++) {
    uthread_join(pt[i], 0);
  }



  printf ("Times with 1 person who identifies as male   %d\n", occupancyHistogram [MALE]   [1]);
  printf ("Times with 2 people who identifies as male   %d\n", occupancyHistogram [MALE]   [2]);
  printf ("Times with 3 people who identifies as male   %d\n", occupancyHistogram [MALE]   [3]);
  printf ("Times with 1 person who identifies as female %d\n", occupancyHistogram [FEMALE] [1]);
  printf ("Times with 2 people who identifies as female %d\n", occupancyHistogram [FEMALE] [2]);
  printf ("Times with 3 people who identifies as female %d\n", occupancyHistogram [FEMALE] [3]);
  printf ("Waiting Histogram\n");
  for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Number of times people waited for %d %s to enter: %d\n", i, i==1?"person":"people", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Number of times people waited more than %d entries: %d\n", WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
