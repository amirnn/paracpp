# Parallel Programming Playground

Based on this playlist by Tom Nurkkala: [Link to the playlist]([https://](https://www.youtube.com/watch?v=bHgz3KLnQlY&list=PLG3vBTUJlY2HdwYsdFCdXQraInoc3j9DU))

And the books:

1. C++ concurrency in Action by Anthony Williams
2. GPU Parallel Program Development Using CUDA By Tolga Soyata

## Dependency installation

I am using conan for dependency management. To install dependencies,

```shell
# Execute the commands below
$ cd conan/deploy
# Choose profile based on platform and desired compiler
$ conan install .. --profile=../profile/linux/clang --build=missing
```

Dependencies:

1. DOCTEST

## Topics

1. Flynn's Taxonomy
2. Performance Metrics
3. Memory mangement in Unix/Linux
4. Process and Fork
5. POSIX threads aka. pthreads
6. C++11 threads
7. Make
8. OpenMP
9. Graphics
10. CUDA
11. Parallel Algorithms

### 1. Flynn's Taxonomy

Parallel and distributed systems are classified into 4 different categories based on instruction and memory handling. That is the instruction can be carried out in a Single or Multiple fashion. Meaning, at a given time, a single stream of instruction or multiple stream instructrions can be in the midst of execution. This also applys to the Memory access. At a given time a single or multiple data are being read/write. Considering these different possibilites, we have 4 different classes refered to as Flynn's Taxonomy.

1. SISD (Single Instruction Stream using Single Data; Conventional one processor, sequential machine.)
2. SIMD (Single Instruction Stream using Multiple Data; Used in GPUs, each instruction is applied on a vector.)
3. MISD (Multiple Instruction Streams using Single Data; Fault tollerant system design, increase reliability.)
4. MIMD (Multiple Instuction Streams using Multiple Data; Multicore CPUs, Distributed Systems.)

In MIMD each processor can execute its own discrete instruction, However, in SIMD it is the same instruction which gets executed in different processing units. This makes MIMD more flexable and Generic and also more complex.

#### MIMD

The MIMD class gets divided into two different sub-classes.

1. Shared Memory (All CPUs can access the memory, Simpler, Limited Scalability, Harder to manifacture since the memory access dominates the chip, Race Conditions.)
2. Distributed Memory (Master-Worker and Symmetric)

The Shared Memory is the default architecture for most common processors these days. Each processor has various CPUs and they share a common memory. They are independent processors and hence we have MI; They got different caches (L1 and L2) which are exclusive and unique to them, but they share the same level 3 cache (L3) and memory space (main memory).

Race condition is when two processing units are trying to read/write the same part of memory. This can result in undefined and undeterministic outcomes.

The Distributed Memory also gets further divided into Master-Worker and Symmetric sub-classes.
In Distributed Memory class, each CPU has a local memory and the cpus and the memories are connected to each other through a data line. For example a cluster of workstations connected to eachother using a network e.g., a LAN or an ethernet cable. The CPUs can be symmetric, meaning all CPUs are equivalent (and can do anything hence, MI).
Or, Master-Worker, where different CPUs perform different tasks; In GPU system, GPU a "worker". In this case there is an architectural difference between CPUs. One is a normal CPU and the other is a GPU. But this class can be applied into several CPUs in which one or few of them are responsible for orchestrating and managing the work of the remaining bunch of CPUs. In this case the nature of task is different as opposed to the architecture and intrinsics of the CPUs.

#### Newer Classes

There have been newer classes emerged since the Flynn's original purposal for the classes.

1. SIMT (Single Instruction, Multiple Thread; Introduced by nVidia, allows threads to diverge and converge, simplifies programming since it is higher level, diverging threads reduce performance.) (Similar to SIMD)
2. SPMD (Single Program, Multiple Data, Each processor runs same program not the same thread as in SIMD, Independent Execution/Control (Program Counter) per CPU.) (Similar MIMD)

### 2. Performance Metrics

#### Speedup (S)

t_1: runtime on \#1 processor using best sequential algorithm. t_p: run time on \#p processors using the parallel algorithm.

`S = t_1/t_p;`

* S is in range \[1,p\]
* S = p is called linear speadup - very rare
* t_p is measured in "wall clock time"
* Notoriously hard to measure accurately
* Influenced bt programmer, compiler, OS, load etc.
* Must test under identical hardware and software, identical operational conditions (e.g., load)
* Use fastest sequential algorithm available

#### Efficiency

Expresses how well a parallel algorithms makes use of available computing resources.

p: \#processors

`E = S/p = t_1/(p . t_p)`

* E is in reange \(0, 1\]
* sometimes expressed as percentage
* Linear speedup gives E = p/p = 1 (100%) very rare
* Always run-time overhead
  * communication overhead
  * contention over shared memory
  * unbalanced workload -> idle CPUs

### 3. Memory mangement in Unix/Linux

When an executable wants to start running in Unix based Operating Systems it firstly gets written to Memory and Operating System issues the executable (application), a virtual memory which behaves as if it is infinitly big. This part of memory gets managed by the operating system. This way we create an isolation for the program and increase the security of the execution. The reason for this is that the program can not access parts of the memory which does not belong to it. However, in a case the program needs more memory it never runs out of memory. In this case the operating system gaurantess that it will provide the needed memory the program. In case the demanded memory was larger than the systems capabilities meaning it is greater than the available free memory of the system (RAM in modern computers), the program runs into stackoverflow error.

In Distributed Memory class we do not have the problem of race condition and memory access; however, now we need to come up with some way to access and share data between CPUs using the network

#### 3.1 Memory Layout

The layout of memory in a Unix based OS:

| Code | Static Memory (Data) | Heap ...->|...|<-...Stack|

Lower Memory ---------------------> Higher Memory

The Code gets and the Static Memory are located in lower address spaces and the Stack, in the higher parts of the memory. Heap grow towards higher Memory adresses and the Stack grows towards lower addresses. The memory space between Stack and Heap is managed by the OS and it is usually considerably large that we do not run into issues of these two memory addresses colliding.

#### 3.2 Static Memory

The executable consits of different parts. The Static part which its values and sizes are known and determind at the compile time. This part consists of Static Memory and the Code. This part of the application is called static part. which gets uploaded to the memory in static part of the memory in applicaion's memory space. The size of Static part of memory never changes and stays the same. However, this does not mean the values at this part of memory are immutable. Global variables and variables marked with static get saved in this part of memory and they do not get deleted until the termination of the program.

#### 3.3 Heap And Stack

Heap and Stack are part of the memory which their size can be changed during runtime. Runtime means the execution of the program. The Heap is also called the Dynamic Memory which can be increased by the programmer. For example in C we can use `malloc()` call and in C++ we can do it using `new` call or operator. The Stack and its size gets determined by the compiler at compile time. For this reason anything that needs to be defined in stack, needs to have a known and determined size. This is the reason for example why we need to know the size of arrays in C and C++. Since, arrays get allocated in the stack. Stack is last in first out type of data structure and it is much faster compared to heap. That is why accessing data which is on the stack is reletivly faster than accessing data which is located in heap (dynamic memory).

### 4. Unix Processs and Fork System Call

The fork() call provides us with the option to be able to create a new duplicate process. This approach has a considerbale amount of overhead and is not necessaraly the best approach. That is why the pthreads got theorized, standardized and implemented.

* Complete copy of all code and data
* Return value
  * if in Parent: pid of child
  * if in Child: 0
* Child behavior
  * Typical: call `exec` system call to load/run new program
  * Parallel: select and solve problem parition
* Analysis
  * Maximum flexibility: can `exec` new program
  * Can run on own core: parallelism
  * Considerable memory overhead
  * Time consuming

```c
// example

#include <sys/types.h>
#include <unistd.h>

int main (int argc, const char ** argv){
  
  pid_t child_id;
  child_id = fork();
  if (child_id == 0){
    std::cout << "This is a child process." << std::endl;
  } else {
    std::cout << "This is the parent process." << std::endl;
  }
  return 0;
}
```

#### fork() in Memory

Fork duplicates the process and creates a child process. Initially after the fork() call and initialization of the child process the Code, Data and Heap parts of the memory are shared between Child process and the Parent process. Eventhough, both process get their own distinct and seperate Stacks. This is due to the virtaul memory and that the child process gets mapped to the same virutal memory space (page of memory) of the parent memory space. Hence, it can access these shared parts. But as soon as one of the processes wants to make a change in this page of memory, that is altering a value in Heap or Data part of the memory space (Code would not change) or the child process starts `exec` by executing, the opertaing system stops the process and creates a new memory page (virtual memory space) and seperates these two processes; The OS reinitilizes the Heap and Data part of the memory for the child process such that each process get their own private memory space. This results in lack of access between these two processes from that point on. So, we will have read access in the beginning and no write acces using this method. But as soon as the child process gets executed we lose access to the shared memory. In other words we can not utilizer the shared memory since we can not execute any additional code and when we do we lose access to the shared data.

|Code|Data|Heap|Stack| ... |Code|Data|Heap|Stack|

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

### 5. POSIX threads aka. pthreads and Shared Memory Programming Model

The POSIX threads are the standard and traditional way of creating threads in UNIX based operating systems. They are much lighter and have less overhead than using `fork()` to create execution units.

The static part of the memory address (Code + Data) and the Heap (dynamic memory) of the program gets shared between pthreads. However, each thread gets its own exclusive Stack in the memory. This makes pthreads of Shared Memory sub-class in the MIMD class of Flynn's division of Parallel Systems.

* Lightweight Process
* Multiple threads per process
* Managed by OS
* Analysis
  * Only need to allocate per-thread stack
  * Stack private to each thread
  * Can run on own core: parallelism
  * \[x\] Memory shared among threads: troublesome

|Code|Data|Heap|...|Stack|...|Stack|

#### 5.1 Thread Implemetations

1. POSIX Threads (pthread)
2. C++11 Threads
   1. std::thread
3. Qt Threads
4. Boost Threads

```c++
// example of C++ threads
#include <thread>
#include <iostream>

void hello(){ std::cout << "Hello!" << std::endl; }

int main() {
  thread thr {hello};
  std::cout << "Hello form Parent!" << std::endl;
  thr.join(); // Wait for the thread thr to finish. This is a blocking call.
  return 0;
}
```

#### POSIX threads

Note: For compiling a program with pthreads one needs to link it to program using `-lpthread`.

```c
// a cheat sheet for pthread creation and handling
#include <pthread>

/**
 * @brief Creates a pthread.
 * @arg thread: pthread_t * the pointer to created thread by this call.
 * @arg attr: attributes of the thread we need to start usually is left as NULL
 * @arg start_routine: A Generic fucntion pointer that will get exectued 
 * once the thread starts
 * @arg arg: a generice pointer to the argument of the start_routine funtion.
 * @returns 0 if there was an error.
*/
pthread_create( pthread_t *thread,
                const pthread_attr_t *attr, 
                void *(*start_routine) (void *), 
                void *arg);


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

### 6. C++11 threads

This chapter is based on the books Stroustroup Bjourne A Tour of C++, C++ Concurrency in Action which demonsterate C++11 thread library. This library is implemented in C++ Standard runtime library for each platform and hence we can use it in a crossplatform manner. It abstracts away some of the details therefore makes life of the programmer much easier.

## License

MIT Based.
