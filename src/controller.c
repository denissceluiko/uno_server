#include "../include/message_struct.h"
#include "../include/client.h"
#include "../include/memory_controller.h"
#include "../include/response.h"
#include <string.h>

void controller_cmd_join(int player_id, struct Message* msg)
{
    char name[PLAYER_NAME_SIZE];
    memcpy(&name, msg->data, PLAYER_NAME_SIZE);

    // if (client_name_exists(name) > 0) {
    //     client_send_message(player_id, "Username already taken");
    //     controller_disconnect()
    //     return;
    // } 

    client_join(player_id, name);
    client_change_mode(player_id, 1);
}

void controller_cmd_loby(int player_id)
{
    char payload[MESSAGE_DATA_SIZE];
    int offset = 0, client_count = memory_get_client_count();

    memcpy(payload + offset, SERVER_NAME, SERVER_NAME_MAX_LENGTH);
    offset += SERVER_NAME_MAX_LENGTH;
    memcpy(payload + offset, SERVER_MOTD, SERVER_MOTD_MAX_LENGTH);
    offset += SERVER_MOTD_MAX_LENGTH;
    memcpy(payload + offset, &client_count, sizeof(char));
    offset += sizeof(char);

    response_send(player_id, "LOBY", payload);
}