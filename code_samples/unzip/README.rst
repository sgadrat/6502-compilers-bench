unzip sample
============

This is a real-life inspired benchmark.

The function tested is a naive implementation of the extraction of some bytes from (lightly) compressed data.

In real life, this function ended being implemented in assembly. So there is an assembly variant available.

Run the bench
-------------

cc65, 6502-gcc, vbcc::

	$ compilers/<compiler>/bench --options="<your_favorite_optims>" code_samples/unzip/unzip.c code_samples/unzip/unzip_data.s

KickC::

	$ compilers/kickc/bench code_samples/unzip/kickc_unzip_data.c code_samples/unzip/kickc_unzip.c

Assembly::

	$ compilers/asm/bench code_samples/unzip/unzip.s code_samples/unzip/unzip_data.s
