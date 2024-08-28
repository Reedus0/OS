#pragma once

#include <stdint.h>
#include <stddef.h>

#include "kernel/shell.h"

shell_command sh_hello(char* command);
shell_command sh_clear(char* command);
shell_command sh_module(char* command);
shell_command sh_cd(char* command);
shell_command sh_ls(char* command);