#ifndef CONTROLLER
#define CONTROLLER

#include "message_struct.h"

void controller_cmd_join(int player_id, struct Message* msg);
void controller_cmd_loby(int player_id);

#endif