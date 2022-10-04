#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_RECORDS 48000
#define NUM_THREADS 8
#define NUM_RECORDS_PER_THREAD (NUM_RECORDS / NUM_THREADS)

#define ALWAYS_LOCK 1
#define FAKE_WORK usleep(1000)

// MOCK APPLICATION DATA STRUCTURE
typedef struct {
  int value;
} record_t;

// Check for matching reocord_t's
bool records_match(record_t *r1, record_t *r2) {
  return r1->value == r2->value;
}

typedef struct {
  int tid;          // thread id
  record_t *start;  // start record
  int count;        // record count
  record_t *target; // match target
} thread_args_t;

int main(int argc, char const *argv[]) { return 0; }
