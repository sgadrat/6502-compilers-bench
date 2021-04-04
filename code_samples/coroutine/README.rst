Coroutine sample
================

This sample is the implementation of a coroutine, as described here: https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html

This bench exercize a rarely used feature of C: switch statements can jump to a "case" right in the middle of a loop.

``coroutine.c`` goes one step further, adding constant arithmetic in case statements, to avoid needing two-bytes "step" variable.

``coroutine_easy.c`` is the one to test with most compilers.

Run the bench
-------------

6502-gcc, cc65::

	$ compilers/<compiler>/bench --options="<your_favorite_optims>" code_samples/coroutine/coroutine_easy.c

6502-gcc, KickC::

	$ compilers/<compiler>/bench --options="<your_favorite_optims>" code_samples/coroutine/coroutine.c
