#include "../include/constants.h"
#include "../include/game.h"
#include "../include/memory_controller.h"
#include <stdio.h>
#include <unistd.h>

void gameloop(int argc, char* argv[])
{
    printf("Gameloop starting.\n");

    game_init();

    while(1) {
        game_process_messages();
        // game_start_lobby();
        // game_start_play();
        // game_clients_print();
        sleep(1);
    }
}