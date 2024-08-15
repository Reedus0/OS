#include "clear.h"
#include "include/print.h"

shell_command sh_clear() {
    print_clear();
    return 0;
}