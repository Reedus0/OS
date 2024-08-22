#pragma once

struct list {
	struct list* prev;
	struct list* next;
};
typedef struct list list_t;

void list_insert_after(list_t* base_list, list_t* new_list) {
	new_list->next = base_list->next;
	base_list->next = new_list;
	new_list->prev = base_list;
}

void list_remove(list_t* removed_list) {
	removed_list->prev->next = removed_list->next;
	if (removed_list->next != NULL) {
		removed_list->next->prev = removed_list->prev;
	}

	removed_list->prev = NULL;
	removed_list->next = NULL;
}