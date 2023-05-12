#ifndef CLIENT
#define CLIENT

#include "message_struct.h"

void client_init(int id);
void client_change_mode(int id, int mode);
int client_get(int id);
int client_get_state(int id);
void client_set_state(int id, int state);

int client_prejoin(int id);
int client_join(int id, const char* name);
void client_leave(int id);

void client_print(int id);
void client_process_message(int id);
void client_cleanup(int id);
void client_send_response(int id, char* type, char* payload);

int client_has_incoming_message(int id);
int client_has_outgoing_message(int id);
void client_set_incoming_message(int id, struct Message* msg);
void client_pull_incoming_message(int id, struct Message* msg);
void client_set_outgoing_message(int id, struct Message* msg);
void client_pull_outgoing_message(int id, struct Message* msg);

#endif