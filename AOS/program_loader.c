#include "program_loader.h"

#include "byte.h"
#include "config.h"
#include "memory.h"
#include "utils.h"

byte next_pid = 1;

insufficient_space_strategy current_insufficient_space_strategy = PANIC_ON_FAIL;

/**
 * Loads a program into memory from an array.
 * Returns .
 * @param program The program to load.
 * @param size The size of program.
 * @return The first page number into which the program was allocated.
 */ 
byte load_program(byte program[], int size) {
    // Integer division with ceiling truncation
	byte pages_required = (size + PAGE_SIZE - 1) / PAGE_SIZE;

	byte page_start = 0xFF;
	byte consecutive_pages = 0;
	// Find enough consecutive empty pages
	for (byte i = 0; i < PAGE_COUNT; i++) {
		byte alloc = avm_page_allocations[i];

		if (alloc == 0) {
			consecutive_pages++;
			if (consecutive_pages == pages_required) {
				page_start = (i + 1) - pages_required;
			}
		} else {
			consecutive_pages = 0;
		}
	}

	// If we never found enough pages, use our recovery strategy
	if (page_start == 0xFF) {
		switch (current_insufficient_space_strategy) {
			case PANIC_ON_FAIL:
				panic();
				break;
			case DESTROY_EVERYTHING:
				wipe_memory();
				break;
			case SILENT_ERROR: break;
		}
		return 0xFF;
	}

	// Copy the program into the page(s)
	for (int i = 0; i < size; i++) {
		avm_memory_pool[page_start * PAGE_SIZE + i] = program[i];
	}

	return page_start;
}