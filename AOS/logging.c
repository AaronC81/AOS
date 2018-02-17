#include "logging.h"

#include <string.h>

/**
 * Writes a message to the logs.
 * This may cause older messages to be pushed out.
 * Warning: Due to the memory shifting done on the log, logging is expensive.
 */
void write_log(const char* message) {
#if LOG_ENABLED == 1
	int len = strlen(message) + 1;
	
	// Shift array elements along
	for (int i = LOG_LENGTH - len; i >= 0; i--) {
		logs[i + len] = logs[i];
	}

	// Insert the new log message
	memcpy(logs, message, len);
#endif
}