#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"

struct stream {
    byte* buffer;
    bool ended;
    size_t size;
};
typedef struct stream stream_t;

void stream_add_byte(stream_t* stream, char new_byte) {
    if (stream->ended) return;
    stream->buffer[stream->size] = new_byte;
    stream->size += 1;
}

void stream_delete_byte(stream_t* stream) {
    if (stream->ended) return;
    if (stream->size > 0) {
        stream->buffer[stream->size] = 0;
        stream->size -= 1;
    }
}

byte stream_get_last_byte(stream_t* stream) {
    if (stream->ended) return 0;
    return stream->buffer[stream->size - 1];
}

void stream_ended(stream_t* stream) {
    stream->ended = 1;
}

bool stream_get_ended(stream_t* stream) {
    return stream->ended;
}

size_t stream_get_size(stream_t* stream) {
    return stream->size;
}

void clear_stream(stream_t* stream) {
    if (stream->ended) return;
    for (size_t i = 0; i < stream->size; i++) {
        stream->buffer[i] = 0;
    }
    stream->size = 0;
}