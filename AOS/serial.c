#include "serial.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/io.h>
#include <util/setbaud.h>
#include <stdio.h>

FILE serial_out = FDEV_SETUP_STREAM(serial_put, NULL, _FDEV_SETUP_WRITE);

/**
 * Initializes the serial port.
 */
void serial_init(void) {
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif

	// Use 8-bit data
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	// Enable TX and RX
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

/**
 * Prints a character over serial.
 */
void serial_put(char c) {
	// AtmelStudio shows error here - this code is valid, ignore it
	// Wait until the data register becomes empty
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

/**
 * Overrides stdout to print to serial instead.
 */
void serial_hook_stdout() {
	stdout = &serial_out;
}