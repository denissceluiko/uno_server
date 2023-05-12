#include "../include/message_struct.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

void message_encode(struct Message* message, const char* type, const int player_id, const char* data)
{
    memcpy(message->type, type, sizeof(char) * MESSAGE_TYPE_SIZE);
    message->player_id = (int) htonl(player_id);
    memset(message->data, 0, sizeof(char) * MESSAGE_DATA_SIZE);
    memcpy(message->data, data, sizeof(char) * MESSAGE_DATA_SIZE);
}

void message_decode(struct Message* message, char* type, int* player_id, char* data)
{
    memcpy(type, message->type, sizeof(char) * MESSAGE_TYPE_SIZE);
    type[MESSAGE_TYPE_SIZE] = '\0';

    *player_id = (int) ntohl(message->player_id);
    memcpy(data, message->data,  sizeof(char) * MESSAGE_DATA_SIZE);
    data[MESSAGE_DATA_SIZE] = '\0';
}

void message_print(struct Message* message)
{
    char type[MESSAGE_TYPE_SIZE + 1], data[MESSAGE_DATA_SIZE + 1];
    int player_id;
    message_decode(message, type, &player_id, data);

    printf("Message\n");
    printf("Type: %s\tPlayer: %d\tMessage: %s\n", type, player_id, data);
}