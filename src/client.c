#include "../include/constants.h"
#include "../include/controller.h"
#include "../include/client_struct.h"
#include "../include/memory_controller.h"
#include "../include/memory_struct.h"
#include "../include/message.h"
#include "../include/response.h"
#include "../include/router.h"
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>


struct Client* client_get(int id) {
    if (id >= MAX_CLIENTS) return NULL;

    struct Memory* shared_memory = memory_get_shared();
    return &(shared_memory->clients[id]);
}

int client_get_state(int id) {
    if (id >= MAX_CLIENTS) return 0;

    struct Client* client = client_get(id);
    return client->state;
}

void client_set_state(int id, int state) {
    if (id >= MAX_CLIENTS) return;

    struct Client* client = client_get(id);
    client->state = state;
}

void client_init(int id)
{
    if (id >= MAX_CLIENTS) return;

    struct Client* client = client_get(id);
    client->state = STATE_DOES_NOT_EXIST;
    client->id = 0;
    char sem_name[20];
    sprintf(sem_name, "/game_sem_cli_%d", id);
    client->semaphore = sem_open(sem_name, O_CREAT, 0644, 1);

    if (client->semaphore == SEM_FAILED) {
        printf("Semaphore init for %d failed. %s", id, strerror(errno));
        exit(1);
    }

    int sem_gv = 0, sem_val = -1;
    sem_gv = sem_getvalue(client->semaphore, &sem_val);
    printf("SEM GV for %d %d @ %i\n", id, sem_gv, sem_val);
}

int client_prejoin(int id)
{
    if (id >= MAX_CLIENTS) return -1;
    if (client_get_state(id) != 0) return -1;

    struct Client* client = client_get(id);
    client->id = id;
    client->state = STATE_BEFORE_INIT;

    return 0;
}

int client_join(int id, const char* name)
{
    if (id >= MAX_CLIENTS) return -1;
    if (client_get_state(id) != STATE_BEFORE_INIT) return -1;

    struct Client* client = client_get(id);
    memcpy(client->name, name, PLAYER_NAME_SIZE);
    client->id = id;
    client->state = STATE_INITIALIZED;
    return 0;
}

void client_leave(int id)
{
    if (id >= MAX_CLIENTS) return;
    if (client_get_state(id) == 0) return;

    struct Client* client = client_get(id);
    client->id = 0;
    client->state = STATE_DOES_NOT_EXIST;
    memset(client->name, 0, PLAYER_NAME_SIZE);
}

void client_cleanup(int id)
{
    if (id >= MAX_CLIENTS) return;

    struct Client* client = client_get(id);
    sem_close(client->semaphore);
}

void client_print(int id) 
{
    if (id >= MAX_CLIENTS) return;
    if (client_get_state(id) < 1) return;

    struct Client* client = client_get(id);
    printf("CLI: %d; State: %d, Name: %s\n", client->id, client->state, client->name);
}

int client_has_incoming_message(int id)
{
    if (id >= MAX_CLIENTS) return 0;

    struct Client* client = client_get(id);
    if (client->message_in.type[0] == '\0') return 0;
    return 1;
}

int client_has_outgoing_message(int id)
{
    if (id >= MAX_CLIENTS) return 0;

    struct Client* client = client_get(id);
    if (client->message_out.type[0] == '\0') return 0;
    return 1;
}

void client_set_incoming_message(int id, struct Message* msg)
{
    if (id >= MAX_CLIENTS) return;

    struct Client* client = client_get(id);

    printf("SEM IS waiting for %d\n", id);
    sem_wait(client->semaphore); // Wait
    printf("SEM IS locked for %d\n", id);
    // Clean
    memset( &client->message_in, 0, sizeof(struct Message) );
    // Make a copy
    memcpy( &client->message_in, msg, sizeof(struct Message) );
    sem_post(client->semaphore); // Release
    printf("SEM IS released for %d\n", id);
}

void client_pull_incoming_message(int id, struct Message *msg)
{
    if (id >= MAX_CLIENTS) return;

    struct Client* client = client_get(id);
    printf("SEM IP waiting for %d\n", id);
    sem_wait(client->semaphore); // Wait
    printf("SEM IP locked for %d\n", id);
    // Make a copy
    memcpy(msg, &client->message_in, sizeof(struct Message));
    // Delete original
    memset( &client->message_in, 0,  sizeof(struct Message));
    sem_post(client->semaphore); // Release
    printf("SEM IP released for %d\n", id);
}

void client_set_outgoing_message(int id, struct Message* msg)
{
    if (id >= MAX_CLIENTS) return;

    struct Client* client = client_get(id);
    printf("SEM OS waiting for %d\n", id);
    sem_wait(client->semaphore); // Wait
    printf("SEM OP locked for %d\n", id);
    // Clean
    memset( &client->message_out, 0,  sizeof(struct Message));
    // Make a copy
    memcpy(&client->message_out, msg, sizeof(struct Message));
    sem_post(client->semaphore); // Release
    printf("SEM OS released for %d\n", id);
}

void client_pull_outgoing_message(int id, struct Message *msg)
{
    if (id >= MAX_CLIENTS) return;

    struct Client* client = client_get(id);
    printf("SEM OP waiting for %d\n", id);
    sem_wait(client->semaphore); // Wait
    printf("SEM OP locked for %d\n", id);
    // Make a copy
    memcpy(msg, &client->message_out, sizeof(struct Message));
    // Delete original
    memset( &client->message_out, 0,  sizeof(struct Message));
    sem_post(client->semaphore); // Release
    printf("SEM OP released for %d\n", id);
}

void client_process_message(int id)
{
    if (id >= MAX_CLIENTS) return;
    if (client_get_state(id) < 1) return;
    if (client_has_incoming_message(id) == 0) return;

    struct Message* msg = malloc(sizeof(struct Message));
    client_pull_incoming_message(id, msg);
    router_handle(id, msg);
    free(msg);
}

void client_change_mode(int id, int mode)
{
    if (mode == 1) { // LOBY
        client_set_state(id, STATE_NOT_READY);
        controller_cmd_loby(id);
    } else if (mode == 2) { // GAME
        // controller_cmd_game(id);
    }
}