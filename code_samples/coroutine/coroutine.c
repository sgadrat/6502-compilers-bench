#include <stdint.h>

#define COROUTINE_BEGIN uint16_t const coroutine_origin_line = __LINE__; switch (step) { case 0:
#define COROUTINE_END }
#define yield(r) step = (__LINE__ - coroutine_origin_line); return (r); case (__LINE__ - coroutine_origin_line):

uint8_t coroutine() {
	static uint8_t step = 0;
	static uint8_t i;
	static const char* data = "hello";

	COROUTINE_BEGIN
	while (1) {
		for (i = 0; i < 4; ++i) {
			yield((uint8_t)data[i]);
		}
	}
	COROUTINE_END
}

void benched_routine() {
	uint8_t* buffer = (uint8_t*)0x0400;
	for (uint8_t i = 0; i < 128; ++i) {
		buffer[i] = coroutine();
	}
}
