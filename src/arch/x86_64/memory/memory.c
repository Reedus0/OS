#include "memory.h"
#include "paging.h"

shell_command sh_memory(char* command) {
    printf("Available pages: %d\n", g_available_pages);
    return 0;
}