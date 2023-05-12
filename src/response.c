#include "../include/client.h"
#include "../include/controller.h"
#include "../include/message_struct.h"
#include "../include/message.h"
#include <string.h>

void response_send(int player_id, char* type, char* payload) {
    if (player_id >= MAX_CLIENTS) return;
    if (client_get_state(player_id) < 1) return;

    struct Message msg;
    message_encode(&msg, type, player_id, payload);
    client_set_outgoing_message(player_id, &msg);
}
