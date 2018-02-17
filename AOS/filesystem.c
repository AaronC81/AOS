#include "filesystem.h"

#include <string.h>
#include <avr/eeprom.h>

/**
 * Reads all files from the file system and returns them as an array.
 */
file* read_files() {
	for (;;) {
		
	}
}

/**
 * Given a pointer to the beginning of a file, reads the file's information into a file struct.
 */
file read_file(int location) {
	byte filename[8];
	byte ext[3];
	byte flags;
	int len;

	eeprom_read_block(filename, location, 8);
	eeprom_read_block(ext, location + 8, 3);
	flags = eeprom_read_byte(location + 8 + 3);
	eeprom_read_block(&len, location + 8 + 3 + 1, 2);

	file result;
	memcpy(result.name, filename, 8);
	memcpy(result.ext, ext, 3);
	result.flags = flags;
	result.size = len;
	result.content_begin = location + 8 + 3 + 1 + 2;

	return result;
}