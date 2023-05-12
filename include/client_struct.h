#ifndef CLIENT_STRUCT
#define CLIENT_STRUCT
#include "constants.h"
#include "message_struct.h"
#include <semaphore.h>

struct Client {
    int state; // 0 - does not exist; 1 - before init; 2 - initialized; 3 - not ready; 4 - ready
    int id;
    char name[PLAYER_NAME_SIZE];
    sem_t* semaphore;
    struct Message message_in;
    struct Message message_out;
};

#endif