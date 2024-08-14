#pragma once

#include <stdint.h>
#include <stddef.h>

void* kalloc(size_t bytes);
void kfree(void* ptr);