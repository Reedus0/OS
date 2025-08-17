#pragma once

#define container_of(ptr, parent_struct, list_name) (parent_struct*)((char*)ptr - offsetof(parent_struct, list_name))

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

#define align(value, alignment) ((value + alignment - 1) & ~(alignment - 1))