#include "stream.h"

void stream_add_byte(stream_t* stream, char new_byte) {
    *(stream->buffer + stream->size) = new_byte;
    stream->size += 1;
}

void stream_delete_byte(stream_t* stream) {
    if (stream->size > 0) {
        *(stream->buffer + stream->size) = 0;
        stream->size -= 1;
    }
}

byte stream_get_last_byte(stream_t* stream) {
    while (stream->updated == 0);
    stream->updated = 0;
    printf("");
    return *(stream->buffer + stream->size - 1);
}

size_t stream_get_size(stream_t* stream) {
    return stream->size;
}

void stream_update(stream_t* stream) {
    stream->updated = 1;
}

void clear_stream(stream_t* stream) {
    for (size_t i = 0; i < stream->size; i++) {
        *(stream->buffer + i) = 0;
    }
    stream->size = 0;
}