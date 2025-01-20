#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "include/stream.h"

#define STDIN_BUFFER_SIZE 256

bool g_stdin_updated;

void stdin_add_byte(byte character);
void stdin_delete_byte();
byte stdin_get_last_byte();
void stdin_update();
void clear_stdin();