#pragma once

#include <stdint.h>
#include <stddef.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
void io_wait();
void enable_irq();
void disable_irq();