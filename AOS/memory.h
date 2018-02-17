#pragma once

#include "config.h"
#include "byte.h"

/**
 * The registers of the AVM. Some registers are general-purpose, while others are specialized, such
 * as the instruction pointers.
 */
byte avm_registers[32];

/**
 * The memory pool of the AVM. This should NOT be accessed or set directly.
 */
byte avm_memory_pool[PAGE_SIZE * PAGE_COUNT];

/**
 * Stores the current allocation status of all memory pages.
 * 0 means unallocated and 1 means allocated, though 1 may be replaced with a PID later.
 */
byte avm_page_allocations[PAGE_COUNT];

void wipe_memory(void);
byte read_memory(byte page, byte mem_byte);
byte read_instruction_pointer(void);
byte type_of_operand(byte operand);
void advance_instruction_pointer(void);
void advance_and_get_operand(byte *op, byte *type);