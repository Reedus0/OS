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
#define KERNEL_TASK 0xFFFF

enum TASK_STATUS {
    NOT_READY = 0,
    READY = 1,
    EXITED = -1
};

struct task_context {
    struct regs regs;
    uint64_t ip;
};

struct task {
    uint16_t id;
    int (*entry)();
    struct task_context* context;
    void* stack;
    enum TASK_STATUS status;
    list_t list;
};
typedef struct task task_t;

task_t* g_task_list;
task_t g_kernel_task;
uint16_t g_current_task_id = KERNEL_TASK;
uint16_t g_max_task_id = 0;

void task_entry(void* param) {
    int (*func)() = get_context_register();
    func(param);
    exit_task();
}

task_t* create_task(int (*func)(), void* param) {
    task_t* new_task = kalloc(sizeof(task_t));

    new_task->entry = task_entry;
    new_task->id = g_max_task_id;

    g_max_task_id += 1;

    new_task->context = kalloc(sizeof(struct task_context));
    new_task->context->ip = new_task->entry;
    new_task->status = NOT_READY;

    void* stack = kalloc(TASK_STACK_SIZE);

    new_task->stack = stack + TASK_STACK_SIZE;

    init_context(&new_task->context->regs, func, new_task->stack, param);

    if (g_task_list == NULL) {
        g_task_list = new_task;
        list_insert_after(&g_task_list->list, &g_task_list->list);
    } else {
        list_insert_after(&g_task_list->list, &new_task->list);
        g_task_list = new_task;
    }

    return new_task;
}

void delete_task(task_t* task) {
    list_remove(&task->list);

    kfree(task->stack - TASK_STACK_SIZE);
    kfree(task->context);
    kfree(task);
}

task_t* get_task(uint16_t task_id) {

    if (task_id == KERNEL_TASK) {
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

void exit_task() {
    task_t* current_task = get_task(g_current_task_id);

    current_task->status = EXITED;
    while(1);
}

void schedule_task(task_t* task) {
    task->status = READY;
}