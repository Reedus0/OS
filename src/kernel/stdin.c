#include "stdin.h"

#include "include/task.h"

void stdin_add_byte(byte new_byte) {
    task_t* current_task = get_task(g_current_task_id);

    if (current_task->stdin.size < STDIN_BUFFER_SIZE) {
        stream_add_byte(&current_task->stdin, new_byte);
    }
}

void stdin_delete_byte() {
    task_t* current_task = get_task(g_current_task_id);

    if (current_task->stdin.size > 0) {
        stream_delete_byte(&current_task->stdin);
    }
}

byte stdin_get_last_byte() {
    while(g_stdin_updated == 0);
    g_stdin_updated = 0;
    task_t* current_task = get_task(g_current_task_id);
    return stream_get_last_byte(&current_task->stdin);
}

size_t stdin_get_size() {
    task_t* current_task = get_task(g_current_task_id);
    return stream_get_size(&current_task->stdin);
}

void stdin_update() {
    g_stdin_updated = 1;
}

void clear_stdin() {
    task_t* current_task = get_task(g_current_task_id);
    clear_stream(&current_task->stdin);
}