#ifndef MEMORY_CONTROLLER
#define MEMORY_CONTROLLER

struct Memory* memory_get_shared();
int memory_get_client_count();
struct Memory* memory_init();
void memory_decrease_client_count();
void memory_increase_client_count();

#endif