#ifndef MESSAGE_STRUCT
#define MESSAGE_STRUCT
#include "../include/constants.h"

struct Message {
    char type[MESSAGE_TYPE_SIZE];
    int player_id;
    char data[MESSAGE_DATA_SIZE];
    char checksum[MESSAGE_CHKS_SIZE];
};

#endif