#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/kalloc.h"
#include "include/task.h"
#include "include/asm.h"
#include "include/list.h"
#include "include/types.h"
#include "asm/asm.h"

#define TASK_STACK_SIZE 8192

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
short g_max_task_id = 0;

task_t* create_task(int (*func)()) {
    task_t* new_task = kalloc(sizeof(task_t));

    new_task->entry = func;
    new_task->id = (g_max_task_id % 65536) + 16;

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

void exit_task() {
    
}

void schedule_task(task_t* task) {
    task->ready = 1;
}

void run_task(task_t* task) {
    switch_context(&task->context->regs, task->context->ip);
}