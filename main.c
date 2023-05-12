#include "include/constants.h"
#include "include/network.h"
#include "include/memory_controller.h"
#include "include/gameloop.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

int main(int argc, char* argv[]) 
{
    struct Memory* shared_memory = NULL;
    int pid = 0;
    int port;

    if (argc == 2) {
        port = atoi(argv[1]);
    } else {
        port = SERVER_PORT;
    }
    
    printf("Terminal UNO server startup.\n");
    shared_memory = memory_init();

    pid = fork();
    if (pid == 0) {
        network_start(port);
    } else {
        gameloop(argc, argv);
        if (munmap(shared_memory, SHARED_MEMORY_SIZE) == -1) {
            perror("Error freeing shared memory");
            exit(1);
        }
    }

    return 0;
}