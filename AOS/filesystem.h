#pragma once

#include "byte.h"

/**
 * Represents a file in the file system.
 */
typedef struct file {
	byte name[8];
	byte ext[3];
	byte flags;
	int size;
	int content_begin;
} file;

file* read_files();
file read_file(int location);