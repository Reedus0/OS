#pragma once

#define container_of(ptr, parent_struct, list_name) (parent_struct*)((char*)ptr - offsetof(parent_struct, list_name))