#include <stdint.h>

void benched_routine();

volatile uint8_t* const stop_register = (uint8_t*) 0x5000;
void main() {
	benched_routine();
	*stop_register = 0;
}
