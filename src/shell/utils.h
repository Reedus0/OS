#pragma once

#include <stdint.h>
#include <stddef.h>

#include "shell.h"

shell_command sh_hello(char* command);
shell_command sh_clear(char* command);
shell_command sh_module(char* command);
shell_command sh_cd(char* command);
shell_command sh_ls(char* command);
shell_command sh_read(char* command);
shell_command sh_write(char* command);
shell_command sh_mkfile(char* command);
shell_command sh_mkdir(char* command);
shell_command sh_rm(char* command);
shell_command sh_rmdir(char* command);
shell_command sh_read(char* command);
shell_command sh_write(char* command);