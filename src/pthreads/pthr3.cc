#include <stdio.h>
#include <pthread.h>
#include <stlib.h>
#include <uninstd.h>
#include <errno.h>

#define NUM_RECORDS 48000
#define NUM_THREADS 8
#define NUM_RECORSDS_PER_THREAD (NUM_RECORDS / NUM_THREADS) 

#define ALWAYS_LOCK 1
#define FAKE_WORK usleep(1000)

/* MOCK APPLICATION DATA STRUCTURE */
typedef struct {
  int value;
} record_t;


bool records_match(record_t * r1, record_t * r2){
  return r1->value == r2->value;
}


int main(int argc, char const *argv[])
{
  /* code */
  return 0;
}
