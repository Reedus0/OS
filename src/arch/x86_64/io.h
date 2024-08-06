#pragma once

#include <stdint.h>
#include <stddef.h>

int16_t io_in(int16_t port);
void io_out(int16_t port, int16_t data);
void enable_irq();
void disable_irq();