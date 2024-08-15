#include "hello.h"

shell_command sh_hello() {
    printf("Hello, Kernel!\n");
    return 0;
}