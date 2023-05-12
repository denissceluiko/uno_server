#ifndef GAME
#define GAME

void game_clean_client_storage();
void game_seed_client_storage();
void game_init();
void game_cleanup();
void game_clients_print();
void game_process_messages();
int game_new_client_id();

#endif