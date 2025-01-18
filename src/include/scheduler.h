#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/task.h"
#include "include/macro.h"
#include "include/asm.h"
#include "asm/asm.h"

#define MAX_SCHEDULE_COUNT 16

static void idle() {
    while(1);
}

void init_scheduler() {
    printk(INFO, "Initiating scheduler...\n");

    task_t* idle_task = create_task(idle, NULL);
    schedule_task(idle_task);

    printk(SUCCESS, "Initiated scheduler!\n");
}

void clean_exited_tasks() {
    task_t* current_task = g_task_list;
    size_t delete_count = 0;

    while (1) {
        list_t* next = current_task->list.next;
        if (current_task->status == EXITED) {
            delete_task(current_task);
        }
        if (next == NULL) {
            break;
        }
       if (delete_count > MAX_SCHEDULE_COUNT) {
            break;
        }
        current_task = container_of(next, task_t, list);
        delete_count++;
    }
}

uint16_t schedule() {
    task_t* current_task = g_task_list;
    size_t schedule_count = 0;
    
    clean_exited_tasks();

    while (1) {
        list_t* next = current_task->list.next;
        if (current_task->status == READY && current_task->id != 0) {
            g_task_list = container_of(next, task_t, list);
            return current_task->id;
        }
        task_t* next_task = container_of(next, task_t, list);
        if (schedule_count > MAX_SCHEDULE_COUNT) {
            break;
        }
        current_task = container_of(next, task_t, list);
        schedule_count++;
    }
    return 0;
}