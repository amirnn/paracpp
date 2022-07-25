# Parallel Programming Playground
Based on this playlist: [link to...]([https://](https://www.youtube.com/watch?v=bHgz3KLnQlY&list=PLG3vBTUJlY2HdwYsdFCdXQraInoc3j9DU))

And the books:

1. C++ concurrency in Action by Anthony Williams
2. GPU Parallel Program Development Using CUDA By Tolga Soyata

## Dependency installation

I am using conan for dependency management. To install dependencies,

```shell
# execute the commands below
$ cd conan/deploy
$ cd conan install .. --build=missing
```

Dependencies:

1. DOCTEST

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
10. Parallel Algorithms

### 1. Flynn's Taxonomy

Parallel and distributed systems are classified into 4 different categories based on instruction and memory handling. That is Instruction can be carried out in a Single or Multiple fashion. Meaning at a given time, a single instruction or multiple instructrions can be in midst of execution. This also applys to the Memory access. At a given time a single or multiple data are being read/write. Considering these different possibilites, we have 4 different classes refered as Flynn's Taxonomy.

1. SISD (Single Instruction Single Data, Conventional one porcessor design.)
2. SIMD (Single Instruction, Multiple Data, Used in GPUs)
3. MISD (Multiple Instruction Single Data, Fault tollerant system design)
4. MIMD (Multiple Instuction Multiple Data Used in distributed systems)

### 2. Memory mangement in Unix/Linux

When an executable wants to start running in Unix based Operating Systems it firstly gets written to Memory and Operating System issues the executable (application), a virtual memory which behaves as if it is infinitly big. This part of memory gets managed by the operating system. This way we create an isolation for the program and increase the security of the execution. The reason for this is that the program can not access parts of the memory which does not belong to it. However, in a case the program needs more memory it never runs out of memory. In this case the operating system gaurantess that it will provide the needed memory the program. In case the demanded memory was larger than the systems capabilities meaning it is greater than the available free memory of the system (RAM in modern computers), the program runs into stackoverflow error.

#### 2.1 Memory Layout

The layout of memory in a Unix based OS:

| Code | Static Memory | Heap ...->|...|<-...Stack|

Lower Memory ---------------------> Higher Memory

The Code gets and the Static Memory are located in lower address spaces and the Stack, in the higher parts of the memory. Heap grow towards higher Memory adresses and the Stack grows towards lower addresses. The memory space between Stack and Heap is managed by the OS and it is usually considerably large that we do not run into issues of these two memory addresses colliding.

#### 2.2 Static Memory

The executable consits of different parts. The Static part which its values and sizes are known and determind at the compile time. This part consists of Static Memory and the Code. This part of the application is called static part. which gets uploaded to the memory in static part of the memory in applicaions. The size of Static part of memory never changes and stays the same. However, this does not mean the values at this part of memory are immutable. Global variables ad=nd variables marked with static get saved in this part of memory and they do not get deleted until the termination of the program.

#### 2.3 Heap And Stack

Heap and Stack are part of the memory which their size can be changed during runtime. Runtime means the execution of the program. The Heap is also called the Dynamic Memory which can be increased by the programmer. For example in C we can use `malloc()` call and in C++ we can do it using `new` call or operator. The Stack and its size gets determined by the compiler at compile time. For this reason anything that needs to be defined in stack, needs to have a known and determined size. This is the reason for example why we need to know the size of arrays in C and C++. Since, arrays get allocated in the stack. Stack is last in first out type of data structure and it is much faster compared to heap. That is why accessing data which is on the stack is reletivly faster than accessing data which is located in heap (dynamic memory).

### 3. Process and Fork

The fork() call provides us with the option to be able to create a new process. This approach has a considerbale amount of overhead and is not necessaraly the best approach. That is why the pthreads got theorized, standardized and implemented.

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

The POSIX threads are the standard and traditional way of creating threads in UNIX based operating systems. They are much lighter and have less overhead than using `fork()` to create execution units.

The static part of the memory address and the Heap (dynamic memory) of the program gets shared between pthreads. However, each thread gets its own exclusive Stack in the memory. This makes pthreads of a MIMD class in Flynn's division of Parallel Systems.

For compiling a program with pthreads one needs to link it to program using `-lpthread`.

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

## License

MIT Based.
