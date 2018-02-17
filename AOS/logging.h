#pragma once

#include "config.h"

/**
 * The recent log output.
 * Older logs are discarded when new logs come in.
 */
char logs[LOG_LENGTH];

void write_log(const char* message);