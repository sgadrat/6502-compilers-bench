memcpy sample
=============

This sample is the very common task of copying bytes in RAM.

There is different variants. Notably one ``asm_style`` that tries to help the compiler find the good way. Also, there are ``static`` version (that the compiler can inline) and a version in two compile units (that the compiller cannot inline)

Beware of KickC results. It actually doesn't handle compilation units, so the function is always inlined.

Run the bench
-------------

cc65, 6502-gcc, vbcc::

	$ # Version in two compilation units
	$ compilers/<compiler>/bench --options="<your_favorite_optims>" code_samples/memcpy/memcpy_8bit_c_style_bench.c code_samples/memcpy/memcpy_8bit_c_style.c

	$ # Version that can be inlined
	$ compilers/<compiler>/bench --options="<your_favorite_optims>" code_samples/memcpy/memcpy_8bit_c_style_static.c

KickC::

	Same as others, but edit the files first to remove ``const`` keywords. KickC seems to have troubles with pointers to constants.

Assembly::

	$ # Version that use a generic routine
	$ compilers/asm/bench code_samples/memcpy/memcpy_8bit_asm.s

	$ # Version specific to these address (as if it was inlined)
	$ compilers/asm/bench code_samples/memcpy/memcpy_8bit_asm_static.s
