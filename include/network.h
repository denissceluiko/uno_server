#ifndef NETWORK
#define NETWORK

void network_deny_join(int client_socket, const char* reason);
void network_accept_client(int main_socket);
void network_process_client(int client_id, int client_socket);
void network_start();

#endif