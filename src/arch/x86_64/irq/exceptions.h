#pragma once

#include <stdint.h>
#include <stddef.h>

interrupt irq_handle_exception(struct irq_data* irq_data);