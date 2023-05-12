#include "../include/message_struct.h"
#include "../include/controller.h"
#include <string.h>

void router_handle(int player_id, struct Message* msg) {
    if (strstr(msg->type, "JOIN") != NULL) {
        controller_cmd_join(player_id, msg);
    } else if (strstr(msg->type, "LOBY") != NULL) {
        //
    } else {
        
    }
}