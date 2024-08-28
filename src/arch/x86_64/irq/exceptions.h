#pragma once

#include <stdint.h>
#include <stddef.h>

interrupt irq_handle_exception(irq_data_t* irq_data);
interrupt irq_page_fault(irq_data_t* irq_data);