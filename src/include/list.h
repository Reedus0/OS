#pragma once

#include <stddef.h>

#define list_parent(ptr, parent_struct, list_name) (parent_struct*)((char*)ptr - offsetof(parent_struct, list_name))

struct list {
	struct list* prev;
	struct list* next;
};
typedef struct list list_t;

list_t init_list();
void list_insert(list_t* base_list, list_t* new_list);
void list_remove(list_t* removed_list);