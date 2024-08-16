#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"

struct stream {
    byte* buffer;
    byte updated;
    size_t size;
};
typedef struct stream stream_t;

void stream_add_byte(stream_t* stream, byte new_byte);
void stream_delete_byte(stream_t* stream);
byte stream_get_last_byte(stream_t* stream);
size_t stream_get_size(stream_t* stream);
void stream_update(stream_t* stream);
void clear_stream(stream_t* stream);