#include <stdint.h>

static uint8_t* unzip_buffer = 0x0580;
extern const uint8_t* zipped_data;

static void get_unzipped_bytes(const uint8_t* zipped, uint16_t offset, uint8_t count) {
	uint8_t zero_count = 0;
	uint8_t dest = 0;

	// Skip uncompressed bytes until reaching offset
	while (offset != 0) {
		if (zero_count != 0) {
			--zero_count;
			--offset;
		}else {
			if (*zipped != 0) {
				--offset;
				++zipped;
			}else {
				zero_count = *(zipped + 1);
				zipped += 2;
			}
		}
	}

	// Store count uncompressed bytes in mem buffer
	while (dest != count) {
		if (zero_count != 0) {
			unzip_buffer[dest] = 0;
			++dest;
			--zero_count;
		}else {
			if (*zipped != 0) {
				unzip_buffer[dest] =  *zipped;
				++dest;
				++zipped;
			}else {
				zero_count = *(zipped + 1);
				zipped += 2;
			}
		}
	}
}

void benched_routine() {
	get_unzipped_bytes(zipped_data, 0x3a8, 16);
}
