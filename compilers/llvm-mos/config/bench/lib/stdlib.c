#include <stdlib.h>

void abort(void) {
  // Writing to this IO register causes the simulator to abort.
  (*(volatile char *)0x5000) = 0;

  // Prevent the compiler from considering this entire basic block unreachable.
  asm volatile ("");

  __builtin_unreachable();
}

void exit(int status) {
  // Writing to this IO register causes the simulator to exit with the given status.
  (*(volatile char *)0x5000) = 0;

  // Prevent the compiler from considering this entire basic block unreachable.
  asm volatile ("");

  __builtin_unreachable();
}

unsigned long clock() {
  return 0;
}

void reset_clock() {
}
