#include "../include/constants.h"
#include "../include/client.h"
#include "../include/memory_controller.h"
#include "../include/memory_struct.h"
#include <stdio.h>

void game_seed_client_storage()
{
    for (int i=0; i<MAX_CLIENTS; i++) {
        client_init(i);
    }
}

void game_clean_client_storage()
{
    for (int i=0; i<MAX_CLIENTS; i++) {
        client_cleanup(i);
    }
}

void game_init() {
    game_seed_client_storage();
}

void game_cleanup()
{
    game_clean_client_storage();
}


void game_process_messages()
{
    for (int i=0; i<MAX_CLIENTS; i++) {
        if (client_get_state(i) > 0)
            client_process_message(i);
    }
}

void game_clients_print()
{
    int client_count = memory_get_client_count();
    if (client_count > 0) {
        printf("Connected clients: %d\n", client_count);

        for (int i=0; i<MAX_CLIENTS; i++) {
            client_print(i);
        }
    }
}

int game_new_client_id()
{
    for (int i=0; i<MAX_CLIENTS; i++) {
        if (client_get_state(i) == 0)
            return i;
    }

    return -1;
}