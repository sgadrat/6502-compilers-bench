/*
 * A simplified memcpy, with `n` being on one byte and without return value.
 */
#include <stdint.h>

static void memcpy_8bits_c_style(uint8_t* dest, const uint8_t* src, uint8_t n) {
	uint8_t i;

	for (i = 0; i < n; ++i) {
		dest[i] = src[i];
	}
}

void benched_routine() {
	memcpy_8bits_c_style((uint8_t*)0x200, (const uint8_t*)0x400, 200);
}
