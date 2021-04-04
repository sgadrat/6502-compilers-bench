#include <stdint.h>

#define COROUTINE_BEGIN switch (step) { case 0:
#define COROUTINE_END }
#define yield(r) step = __LINE__; return (r); case __LINE__:

uint8_t coroutine() {
	static int step; /*No init so, cc65 puts it in BSS (bench framework do not support DATA segment for now, and BSS is initialized at ZERO)*/
	static uint8_t i;
	static const char* const data = "hello";

	COROUTINE_BEGIN
	while (1) {
		for (i = 0; i < 4; ++i) {
			yield((uint8_t)data[i]);
		}
	}
	COROUTINE_END
}

void benched_routine() {
	uint8_t* const buffer = (uint8_t*)0x0400;
	uint8_t i;
	for (i = 0; i < 128; ++i) {
		buffer[i] = coroutine();
	}
}
