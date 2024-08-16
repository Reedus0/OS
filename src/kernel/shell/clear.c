#include "clear.h"
#include "include/print.h"

shell_command sh_clear(char* command) {
    print_clear();
    return 0;
}