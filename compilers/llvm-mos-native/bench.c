#include <stdint.h>

void benched_routine();

int main() {
	uint8_t volatile * const register_stop = (uint8_t volatile *)0x5000;
	benched_routine();
	*register_stop = 0;
}
