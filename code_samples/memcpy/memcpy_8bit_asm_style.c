/*
 * A simplified memcpy, with `n` being on one byte and without return value.
 */
#include <stdint.h>

void memcpy_8bits_asm_style(uint8_t* dest, uint8_t* src, uint8_t n) {
	do {
		dest[n] = src[n];
		--n;
	}while(n > 0);
}
