# Parallel Programming Playground
Based on this playlist: [link to...]([https://](https://www.youtube.com/watch?v=bHgz3KLnQlY&list=PLG3vBTUJlY2HdwYsdFCdXQraInoc3j9DU))

## Dependency installation

I am using conan for dependency management.

```shell
# execute the commands below
$ cd conan/deploy
$ cd conan install .. --build=missing
```

## Topics

1. Flynn's Taxonomy
2. Memory mangement in Unix/Linux
3. Process and Fork
4. POSIX threads aka. pthreads
5. C++11 threads
6. Make
7. OpenMP
8. Graphics
9. CUDA

### 1. Flynn's Taxonomy

Parallel and distributed systems are classified into 4 different categories based on instruction and memory handling. That is Instruction can be carried out in a Single or Multiple fashion. Meaning at a given time, a single instruction or multiple instructrions can be in midst of execution. This also applys to the Memory access. At a given time a single or multiple data are being read/write. Considering these different possibilites, we have 4 different classes refered as Flynn's Taxonomy.

1. SISD (Single Instruction Single Data, Conventional one porcessor design.)
2. SIMD (Single Instruction, Multiple Data, Used in GPUs)
3. MISD (Multiple Instruction Single Data, Fault tollerant system design)
4. MIMD (Multiple Instuction Multiple Data Used in distributed systems)

### 2. Memory mangement in Unix/Linux

### 3. Process and Fork

```c
// a cheat sheet for Process handling in C/C++
#include <sys/types.h>
#include <unistd.h>

int   fork();
int   execvp(const char *file_name, char *const argv[]);
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
int   kill(pid_t pid, int sig);
int   signal(int signum, void (*sighandler)(int));
```

### 4. POSIX threads aka. pthreads

For compiling once needs to include -lpthread.

```c
// a cheat sheet for pthread creation and handling
#include <pthread>

/**
 * @brief Creates a pthread.
 * @arg thread: pthread_t * the pointer to created thread by this call.
 * @arg start_routine: A Generic fucntion pointer that will get exectued 
 * once the thread starts
 * @arg arg: a generice pointer to the argument of the start_routine funtion.
 * @returns 0 if there was an error.
*/
pthread_create(pthread_t *thread, NULL, void *(*start_routine) (void *), void *arg);


/**
 * @brief Waits for a pthread to finish execution and join.
 * @arg thread: notice the pass by value vs pass by reference from pthread_create. 
 * Here we do not need to modify the thread hence, we pass by value.
 * @arg retval: a pointer to  a generic pointer, the return value (result) of the start_routine
 * @returns 0 if there was an error.
*/
pthread_join(pthread_t thread, void **retval);

pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;

/**
 * We issue the following lock and unlock funtions right before and after the critical read/write
 * action where we need to synchronize the threads and block other threads from accessing the shared data.
*/
pthread_mutex_lock(ptherad_mutex_t *mutex);
pthread_mutex_unlock(ptherad_mutex_t *mutex);

pthread_barrier_t barrier;

pthread_barrier_init(pthread_barrier_t *barrier, NULL, unsigned count);

pthread_barrier_wait(pthread_barrier_t *barrier);

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

pthread_cond_signal(pthread_cond_t *cond);

```
