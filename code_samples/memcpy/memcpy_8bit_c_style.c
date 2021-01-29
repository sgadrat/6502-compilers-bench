/*
 * A simplified memcpy, with `n` being on one byte and without return value.
 */
#include <stdint.h>

void memcpy_8bits_c_style(uint8_t* dest, uint8_t* src, uint8_t n) {
	uint8_t i;

	for (i = 0; i < n; ++i) {
		dest[i] = src[i];
	}
}
