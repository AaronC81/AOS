#pragma once

#include "config.h"
#include <stdio.h>

void serial_init(void);
void serial_put(char c);
void serial_hook_stdout(void);

FILE serial_out;