#include "../include/constants.h"
#include "../include/memory_struct.h"
#include <errno.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Memory* global_shared_memory;

struct Memory* memory_get_shared()
{
    if (global_shared_memory == NULL) {
        printf("Shared memory not initialized.\n");
        exit(1);
        return NULL;
    }

    return global_shared_memory;
}

struct Memory* memory_init()
{
    global_shared_memory = NULL;

    global_shared_memory = mmap(NULL, sizeof(struct Memory), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (global_shared_memory == MAP_FAILED) {
        printf("Shared memory mapping failed. %s\n", strerror(errno));
        exit(1);
    }

    return global_shared_memory;
}

int memory_get_client_count()
{
    struct Memory* memory = memory_get_shared();

    return memory->client_count;
}

void memory_increase_client_count()
{
    struct Memory* memory = memory_get_shared();
    memory->client_count += 1;
}

void memory_decrease_client_count()
{
    struct Memory* memory = memory_get_shared();
    memory->client_count -= 1;
}