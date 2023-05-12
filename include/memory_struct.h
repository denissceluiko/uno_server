#ifndef MEMORY_STRUCT
#define MEMORY_STRUCT
#include "../include/constants.h"
#include "../include/client_struct.h"

struct Memory {
    int client_count;
    struct Client clients[MAX_CLIENTS];
};

#endif