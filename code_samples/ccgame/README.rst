RPG sample
==========

This sample comes from Ilmenit's tutorial on how to optimize for cc65: https://github.com/ilmenit/CC65-Advanced-Optimizations

There is three variants:
 - The one without any optimization
 - The one after all Ilmenit's optimization
 - One optimized for gcc
 - Bonus: some leftovers of various experiments (you are on your own on these ones)

Note: while vbcc runs the examples, it produce invalid result. The page 3xx and the begining of 4xx should contain the entities drawn on screen.

Run the bench
-------------

Without optimization
~~~~~~~~~~~~~~~~~~~~

cc65, 6502-gcc, vbcc-native::

	$ compilers/<compiler>/bench --options="<your_favorite_optims>" code_samples/ccgame/game_01_start.c code_samples/ccgame/dummy_function.c

Optimized for cc65
~~~~~~~~~~~~~~~~~~

cc65, 6502-gcc, vbcc-native::

	$ compilers/<compiler>/bench --options="<your_favorite_optims>" code_samples/ccgame/game_12_final.c code_samples/ccgame/dummy_function.c

Optimized for gcc
~~~~~~~~~~~~~~~~~

6502-gcc, vbcc-native::

	$ compilers/<compiler>/bench --options="<your_favorite_optims>" code_samples/ccgame/game_modern_optims.c code_samples/ccgame/dummy_function.c
