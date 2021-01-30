/*
 * A simplified memcpy, with `n` being on one byte and without return value.
 *
 * This version is intentionaly coded the closer as possible to what one would do in ASM.
 * One side effect is that the `n` value may trick, it will copy n+1 bytes (allowing 256 bytes, but no 0 byte)
 */
#include <stdint.h>

void memcpy_8bits_asm_style(uint8_t* dest, uint8_t* src, uint8_t n) {
	do {
		dest[n] = src[n];
		--n;
	}while(n > 0);
}
