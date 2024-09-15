#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/task.h"
#include "include/macro.h"

void schedule() {
    task_t* current_task = g_task_list;
    while (1) {
        if (current_task->ready) {
            run_task(current_task);
        }
        if (current_task->list.next == NULL) {
            break;
        }
        current_task = container_of(current_task->list.next, task_t, list);
    }
}