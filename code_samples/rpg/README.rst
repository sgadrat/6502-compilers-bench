RPG sample
==========

This sample is a far-fetched RPG engine, with lots of abstraction.

The bench routine call initializers and logic, but in the end, it simply write some bytes in the RAM. This bench is though to show the ability of the compiler to eliminate "useless" abstractions.

There is no assembly version available. Wheter to keep or remove abstraction in assembly is a metter of balance between performance and maintanibility.

Run the bench
-------------

6502-gcc, vbcc::

	$ compilers/<compiler>/bench --options="<your_favorite_optims>" code_samples/rpg/rpg_cc65.c

cc65, KickC::

	$ compilers/<compiler>/bench --options="<your_favorite_optims>" code_samples/rpg/rpg.c
