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
    set_task_register(KERNEL_TASK);
    task_t* idle_task = create_task(idle);
    schedule_task(idle_task);
}

void schedule() {
    printk(INFO, "Running scheduler...\n");
    task_t* current_task = g_task_list;
    while (1) {
        if (current_task->ready && current_task->id != 0) {
            run_task(current_task);
        }
        if (current_task->list.next == NULL) {
            break;
        }
        current_task = container_of(current_task->list.next, task_t, list);
    }
    run_task(get_task(0));
}