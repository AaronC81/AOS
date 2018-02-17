#include "memory.h"

#include "byte.h"
#include "config.h"
#include "avm_register.h"

/**
 * Wipe memory and the allocation status table.
 * This does not reset registers and will usually cause a crash if the IP isn't changed.
 * Use with care!
 */
void wipe_memory(void) {
	for (int i = 0; i < PAGE_SIZE * PAGE_COUNT; i++) {
		avm_memory_pool[i] = 0;
	}

	for (byte i = 0; i < PAGE_COUNT; i++) {
		avm_page_allocations[i] = 0;
	}
}

/**
 * Reads a specific byte from a memory page.
 * @param page The memory page to read from.
 * @param byte The byte to read in the page.
 * @return The byte at the given memory location.
 */
byte read_memory(byte page, byte mem_byte) {
	return avm_memory_pool[page * PAGE_SIZE + mem_byte];
}

/**
 * Returns the byte currently pointed to by the instruction pointer.
 */
byte read_instruction_pointer(void) {
	return read_memory(avm_registers[IPP], avm_registers[IPB]);
}

/**
 * Given a valid operand, returns the type of the operand.
 */
byte type_of_operand(byte operand) {
	return ((operand & 0b11000000) >> 6);
}

/**
 * Advances the instruction pointer one place.
 */
void advance_instruction_pointer(void) {
	avm_registers[IPB]++;
	// If overflowed, increment page
	if (avm_registers[IPB] == 0) {
		avm_registers[IPP]++;
	}
}

/**
 * Advances the instruction pointer and reads the operand at the pointer's new location.
 * The full operand and its type (first 2 bits) are returned through the pointer parameters.
 */
void advance_and_get_operand(byte *op, byte *type) {
	advance_instruction_pointer();
	byte r = read_instruction_pointer();
	*op = r;

	// Sometimes the type is not required, in which case a null pointer is passed
	if (type != NULL) {
		*type = type_of_operand(r);
	}
}