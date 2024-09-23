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
    printk(INFO, "Initiating scheduler...\n");

    task_t* idle_task = create_task(idle, NULL);
    schedule_task(idle_task);

    printk(SUCCESS, "Initiated scheduler!\n");
}

uint16_t schedule() {
    task_t* current_task = g_task_list;
    while (1) {
        list_t* next = current_task->list.next;
        if (current_task->status == READY && current_task->id != 0) {
            g_task_list = container_of(next, task_t, list);
            return current_task->id;
        }
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