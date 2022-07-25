
#include <iostream>

#include <sys/types.h>
#include <unistd.h>

/**
 * @brief Create a fork object
 * @details creates a new child process with its respective id!
 * @return pid_t 
 */
pid_t create_fork(){
    pid_t child_id;
    child_id = fork();
    return child_id;
}

int main(int, char**) {




    std::cout << "Hello, world!\n";
}
