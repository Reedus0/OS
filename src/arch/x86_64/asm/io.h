#pragma once

#include <stdint.h>
#include <stddef.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
uint32_t ind(uint16_t port);
void outd(uint16_t port, uint32_t data);
void io_wait();
void enable_irq();
void disable_irq();