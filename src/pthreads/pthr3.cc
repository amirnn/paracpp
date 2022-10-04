#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_RECORDS 48000
#define NUM_THREADS 8
#define NUM_RECORDS_PER_THREAD (NUM_RECORDS / NUM_THREADS)

#define ALWAYS_LOCK 0
#define FAKE_WORK usleep(1000)

// MOCK APPLICATION DATA STRUCTURE
typedef struct {
  int value;
} record_t;

// Check for matching record_t's
bool records_match(record_t *r1, record_t *r2) {
  return r1->value == r2->value;
}

typedef struct {
  int tid;          // thread id
  record_t *start;  // start record
  int count;        // record count
  record_t *target; // match target
} thread_args_t;

int match_count = 0;
pthread_mutex_t match_count_mutex;

void *find_matches(void *thread_args) {
  thread_args_t *args = (thread_args_t *)thread_args;
  printf("%d: %p[%d]\n", args->tid, (void *)args->start, args->target);

#if ALWAYS_LOCK
  for (int i = 0; i < args->count; i++) {
    bool matched = records_match(&args->start[i], args->target);

    if (matched) {
      pthread_mutex_lock(&match_count_mutex);
      ++match_count;
      FAKE_WORK; // simulate something is being done.
      pthread_mutex_unlock(&match_count_mutex);
    }
  }
#else //! ALWAYS_LOCK
  int local_matches = 0; // local matches
  int busy_count = 0;    // times lock busy
  for (size_t i = 0; i < args->count; ++i) {
    int match = records_match(&args->start[i], args->target);

    if (match) {
      ++local_matches;

      int rtn = pthread_mutex_trylock(&match_count_mutex);
      if (rtn == 0) {
        match_count += local_matches;
        FAKE_WORK;
        pthread_mutex_unlock(&match_count_mutex);
        local_matches = 0;
      } else {
        // Lock is busy
      }
    }
  }
  printf("%d: busy %d times\n", args->tid, busy_count);

  if (local_matches > 0) {
    // No choice but to lock
    pthread_mutex_lock(&match_count_mutex);
    match_count += local_matches;
    pthread_mutex_unlock(&match_count_mutex);
  }

#endif
  return (void *)NULL;
}

void check_thread_rtn(const char *msg, int rtn) {
  if (rtn != 0) {
    fprintf(stderr, "Error: %s (%d)\n", msg, rtn);
    exit(EXIT_FAILURE);
  }
}

int blocking_rand(int min, int max) { return (random() % max) + min; }

int main(int argc, char const *argv[]) {

  pthread_t threads[NUM_THREADS];
  thread_args_t thread_args[NUM_THREADS];

  record_t records[NUM_RECORDS];
  record_t target_record = {7};
  int expected_count = 0;

  // Initialize records.
  srandom(time(NULL)); // seed random
  for (size_t i = 0; i < NUM_RECORDS; ++i) {
    records[i].value = blocking_rand(1, 10);
    if (records_match(&records[i], &target_record)) {
      ++expected_count;
    }
  }

  // Initialize mutex.
  int rtn = pthread_mutex_init(&match_count_mutex, NULL);
  check_thread_rtn("mutex init", rtn);

  // Fire up threads.
  record_t *next_group = records;
  for (int i = 0; i < NUM_THREADS; ++i) {
    thread_args_t &thread_arg_i = thread_args[i];
    thread_arg_i.tid = i;
    thread_arg_i.start = next_group;
    thread_arg_i.count = NUM_RECORDS_PER_THREAD;
    thread_arg_i.target = &target_record;
    pthread_t &thread_i = threads[i];
    rtn = pthread_create(&thread_i, NULL, find_matches, (void *)&thread_arg_i);
    check_thread_rtn("create", rtn);
    next_group += NUM_RECORDS_PER_THREAD;
  }

  // Join all child threads
  for (int i = 0; i < NUM_THREADS; ++i) {
    rtn = pthread_join(threads[i], NULL);
    check_thread_rtn("Join", rtn);
  }

  int exit_value;
  // Output results
  if (match_count == expected_count) {
    printf("Matched %d as expected\n", match_count);
    exit_value = EXIT_SUCCESS;
  } else {
    printf("Matched %d, but expected %d\n", match_count, expected_count);
    exit_value = EXIT_FAILURE;
  }

  return exit_value;
}
