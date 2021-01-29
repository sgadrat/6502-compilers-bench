Goals
=====

Compare code produced by cc65, 6502-gcc, and vbcc

Each with different optim flags, compare code's size and number of cycles used for different examples.

Examples to have:
 - Some CPU intensive task: should show ability to produce nice low-level code
 - Some complex code structure: should show ability to optimize high-level code

Running benchs
==============

::

	./bench --options='-O3' unzip.c

Impacting environment variable

 * CA65_BIN
 * LD65_BIN
 * GCC6502_BIN
 * **VBCC_BIN**: *vc* binary from VBCC toolchain (default: ``vc``)
 * **VBCC**: shall point to VBCC's root dir for vbcc compilation to work
 * EMU65
