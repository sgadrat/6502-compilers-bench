/*
 * A simplified memcpy, with `n` being on one byte and without return value.
 */
#include <stdint.h>

void memcpy_8bits_c_style(uint8_t* dest, uint8_t* src, uint8_t n);

void benched_routine() {
	memcpy_8bits_c_style((uint8_t*)0x200, (uint8_t*)0x400, 200);
}
