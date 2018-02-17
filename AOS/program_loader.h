#pragma once
#include "byte.h"

byte next_pid;

/**
 * Details the recovery strategy to use if there isn't enough memory to load the requested program.
 */
typedef enum {
	/**
	 * Silently errors, not loading the program and prompting allocation methods to indicate somehow that allocation
	 * failed.
	 * For example, methods which should return a page reference may return 0xFF instead.
	 */
	SILENT_ERROR,

	/**
	 * Triggers a panic upon allocation failing, causing the AVR to restart.
	 */
	PANIC_ON_FAIL,

	/**
	 * Mercilessly eliminate every last bit of memory upon allocation failing, by calling wipe_memory.
	 * This is a VERY bad idea and you should probably never use it.
	 */ 
	DESTROY_EVERYTHING
} insufficient_space_strategy;

insufficient_space_strategy current_insufficient_space_strategy;

byte load_program(byte program[], int size);