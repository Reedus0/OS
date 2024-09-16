#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/kalloc.h"
#include "include/task.h"
#include "include/asm.h"
#include "include/list.h"
#include "include/types.h"
#include "include/macro.h"
#include "asm/asm.h"

#define TASK_STACK_SIZE 8192
#define KERNEL_TASK 0xFFFFFFFFFFFFFFFF

struct task_context {
    struct regs regs;
    uint64_t ip;
};

struct task {
    size_t id;
    int (*entry)();
    struct task_context* context;
    void* stack;
    bool ready;
    list_t list;
};
typedef struct task task_t;

task_t* g_task_list;
task_t g_kernel_task;
uint8_t g_max_task_id = 0;

task_t* create_task(int (*func)()) {
    task_t* new_task = kalloc(sizeof(task_t));

    new_task->entry = func;
    new_task->id = g_max_task_id;

    g_max_task_id += 1;

    new_task->context = kalloc(sizeof(struct task_context));
    new_task->context->ip = new_task->entry;

    void* stack = kalloc(TASK_STACK_SIZE);

    new_task->stack = stack + TASK_STACK_SIZE;
    init_context(new_task->context, new_task->stack);

    if (g_task_list == NULL) {
        g_task_list = new_task;
    } else {
        list_insert_after(&g_task_list->list, &new_task->list);
    }
    return new_task;
}

void delete_task(task_t* task) {
    list_remove(&task->list);

    kfree(task->stack - TASK_STACK_SIZE);
    kfree(task->context);
    kfree(task);
}

task_t* get_task(uint64_t task_id) {
    if (task_id == 0xFFFFFFFFFFFFFFFF) {
        return &g_kernel_task;
    }

    task_t* current_task = g_task_list;
    while (1) {
        if (current_task->id == task_id) {
            return current_task;
        }
        if (current_task->list.next == NULL) {
            break;
        }
        current_task = container_of(current_task->list.next, task_t, list);
    }
}

void switch_context(struct task_context* new_context) {
    uint64_t task_id = get_task_register();
    task_t* current_task = get_task(task_id);
    struct regs regs;

    get_regs(&regs);
    current_task->context->regs = regs;

    set_regs(&new_context->regs);
}

void exit_task() {
    
}

void schedule_task(task_t* task) {
    task->ready = 1;
}

void run_task(task_t* task) {
    switch_context(task->context);
    jump(task->context->ip);
}