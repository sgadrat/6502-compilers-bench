/*
 * A dummy function in a separate compilation unit
 * Calling it forces the compiler to flush memory writes in globals and what is pointed.
 */
void dummy_function(void* a, unsigned char* b) {
	(void)a;
	(void)b;
}
