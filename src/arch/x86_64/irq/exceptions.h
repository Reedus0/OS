#pragma once

#include <stdint.h>
#include <stddef.h>

interrupt_t irq_handle_exception(irq_data_t* irq_data);
interrupt_t irq_page_fault(irq_data_t* irq_data);