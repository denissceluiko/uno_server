#ifndef MESSAGE
#define MESSAGE

#include "../include/message_struct.h"

void message_encode(struct Message* message, const char* type, const int player_id, const char* data);
void message_decode(struct Message* message, char* type, int* player_id, char* data);
void message_print(struct Message* message);

#endif