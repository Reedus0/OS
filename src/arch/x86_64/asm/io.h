#pragma once

#include <stdint.h>
#include <stddef.h>

uint8_t in8(uint16_t port);
void out8(uint16_t port, uint8_t data);
uint32_t in32(uint16_t port);
void out32(uint16_t port, uint32_t data);
void io_wait();
void enable_irq();
void disable_irq();