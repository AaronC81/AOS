#pragma once
#include "byte.h"

/**
 * Two-bit prefixes on operands to denote what they represent.
 */
enum operand_type {
	SHORTLIT  = 0b00, // A 6-bit literal
	BYTELIT   = 0b01, // An 8-bit literal, present in the next byte
	DOUBLELIT = 0b10, // A 16-bit literal, present in the next two bytes
	REGREF    = 0b11, // A reference to one of the 32 registers
};

byte get_number(byte type, byte op);
void panic(void);
byte operand_of(byte reg);