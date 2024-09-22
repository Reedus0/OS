#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/task.h"
#include "include/macro.h"
#include "include/asm.h"
#include "asm/asm.h"

static void idle() {
    while(1);
}

void init_scheduler() {
    task_t* idle_task = create_task(idle);
    schedule_task(idle_task);
}

uint16_t schedule() {
    task_t* current_task = g_task_list;
    while (1) {
        if (current_task->status == READY && current_task->id != 0) {
            return current_task->id;
        }
        list_t* next = current_task->list.next;
        if (current_task->status == EXITED) {
            delete_task(current_task);
        }
        if (next == NULL) {
            break;
        }
        current_task = container_of(next, task_t, list);
    }
    return 0;
}