#include "main.h"

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "config.h"
#include "byte.h"
#include "utils.h"
#include "avm_register.h"
#include "avm_instruction.h"
#include "memory.h"
#include "serial.h"
#include "program_loader.h"
#include "logging.h"
#include "filesystem.h"

/**
 * Executes a single instruction.
 * The instruction pointer is NOT advanced to the next instruction, but may be advanced to operands.
 */
void execute_instruction(void) {
	// Read the instruction opcode at the instruction pointer	
	byte opcode = read_instruction_pointer();

	// TODO IMPLEMENT LOAD AND STORE, AND THE OUTS

	switch (opcode) {
		case MOV: {
			// Get the first operand, 'from'
			byte from_op, from_type, from_value;
			advance_and_get_operand(&from_op, &from_type);
			from_value = get_number(from_type, from_op);

			// Get the second operand, 'to'
			byte to_op, to_type;
			advance_and_get_operand(&to_op, &to_type);
			
			// Must assign to a register
			if (to_type != REGREF) panic();

			avm_registers[to_op & 0b00111111] = from_value;
			break;
		}
		case ALCP:
		case DSTP:
			// Currently not implemented, because page ownership isn't
			break;
		case ADD:
		case SUB: {
			// Get the first operand
			byte first_op, first_type, first_value;
			advance_and_get_operand(&first_op, &first_type);
			first_value = get_number(first_type, first_op);
			
			// Get the second operand
			byte second_op, second_type;
			advance_and_get_operand(&second_op, &second_type);

			// Must assign to a register
			if (second_type != REGREF) panic();

			switch (opcode) {
				case ADD:
					avm_registers[second_op & 0b00111111] += first_value;
					break;
				case SUB:
					avm_registers[second_op & 0b00111111] -= first_value;
					break;
			}
			break;
		}
		case INC:
		case DEC: {
			// Get the operand
			byte op, type;
			advance_and_get_operand(&op, &type);

			// Must assign to a register
			if (type != REGREF) panic();

			switch (opcode) {
				case INC:
					avm_registers[op & 0b00111111]++;
					break;
				case DEC:
					avm_registers[op & 0b00111111]--;
					break;
			}
			break;
		}
		case HLT: {
			avm_registers[FL] |= 0b10000000;
			break;
		}
		case OUT: {
			// Get the operand
			byte op, type, value;
			advance_and_get_operand(&op, &type);
			value = get_number(type, op);

			// Output it as an ASCII character
			printf("%c", value);
			break;
		}
		case OUTN: {
			// Get the operand
			byte op, type, value;
			advance_and_get_operand(&op, &type);
			value = get_number(type, op);

			// Output it as a number
			printf("%d", value);
			break;
		}
		case JMP: {
			// Get the operand, which may be negative
			byte op, type, value;
			advance_and_get_operand(&op, &type);
			value = get_number(type, op);

			// Operand may be negative
			signed_byte tc_value = (signed_byte)value; // todo not working - still +254

			// We had to advance to get the operand, so subtract an extra one

			// TODO TODO TODO Handle inter page jumping
			avm_registers[IPB] += tc_value - 1;

			break;
		}
		case SLP: {
			// Get the operand 
			byte op, type, value;
			advance_and_get_operand(&op, &type);
			value = get_number(type, op);

			// TODO Feels too fast - is clock speed set up correctly?
			// Delay for that many ms
			while (value > 0) {
				_delay_ms(1);
				value -= 1;
			}

			break;
		}
		case NOP: break;
		default:
			panic();
	}
}

int main(void)
{
	serial_init();
	serial_hook_stdout();
	printf("\nBooting AOS...\n");

	// Get rootfile information
	file f = read_file(0);
	printf("Rootfile is %s.%s, %d bytes\n", f.name, f.ext, f.size);
	
	// Read rootfile contents
	byte program[f.size];
	eeprom_read_block(program, f.content_begin, f.size);

	// Load rootfile into RAM
	load_program(program, f.size);
	
	avm_registers[IPP] = 0;
	avm_registers[IPB] = 0;

	// Execute the rest of the instructions
	for (;;) {
		execute_instruction();
		advance_instruction_pointer();

		// Check if we need to halt
		if ((avm_registers[FL] >> 7) == 1) {
			// If we have, begin an infinite loop so we halt
			printf("\nHalted\n");
			for (;;);
		}
	}
}
 
