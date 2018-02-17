#include "utils.h"
#include "byte.h"
#include "avm_register.h"
#include "memory.h"

#include <avr/wdt.h>

/**
 * Given an operand and its type, returns the number held in the value.
 * For a SHORTLIT or BYTELIT, it returns the literal.
 * For a REGREF, it reads the register and returns its value.
 * DOUBLELITs are currently not implemented.
 */
byte get_number(byte type, byte op) {
	switch (type) {
		case SHORTLIT:
			return op & 0b00111111;
		case BYTELIT:
			advance_instruction_pointer();
			return read_instruction_pointer();
		case REGREF:
			return avm_registers[op & 0b00111111];
			break;
		default:
			// Doubles not implemented yet
			panic();
			// Panic will never return, so we should never reach here
			return 0;
	}
}

/**
 * Reset the AVR by invoking the watchdog's infinite loop protection.
 */
void panic(void) {
	// This works by configuring the watchdog with 'if we don't do anything for 15ms, reset'
	// Then we enter an infinite loop, firing the watchdog reset
	wdt_enable(WDTO_15MS);
	for(;;) {}
}

/**
 * Given a register, returns a REGREF operand referencing it.
 */
byte operand_of(byte reg) {
	return (REGREF << 6) | reg;
}