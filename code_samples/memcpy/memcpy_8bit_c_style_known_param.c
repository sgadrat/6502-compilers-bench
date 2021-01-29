/*
 * A simplified memcpy, with `n` being on one byte and without return value.
 */
#include <stdint.h>

static void memcpy_8bits_c_style(uint8_t* dest, uint8_t const * src, uint8_t n) {
	uint8_t i;

	for (i = 0; i < n; ++i) {
		dest[i] = src[i];
	}
}

static uint8_t const* my_hot_sprites = (uint8_t const*)0x400;
static uint8_t* oam_mirror = (uint8_t*)0x200;

void copy_buffers() {
	uint8_t const nb_hot_sprites = 1;
	uint8_t const bytes_per_sprite = 4;

	memcpy_8bits_c_style(oam_mirror, my_hot_sprites, nb_hot_sprites * bytes_per_sprite);
}
