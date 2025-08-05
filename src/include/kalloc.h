#pragma once

#include <stdint.h>
#include <stddef.h>

void* kalloc(size_t bytes);
void* kalloc_aligned(size_t bytes, size_t alignment);
void kfree(void* ptr);