#include "hello.h"

shell_command sh_hello(char* command) {
    printf("Hello, Kernel!\n");
    return 0;
}