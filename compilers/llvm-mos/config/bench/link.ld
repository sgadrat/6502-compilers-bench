/* 6502 simulator linker script.
 *
 * Produces a 64KiB memory image file usable with the provided simulator.
 */

OUTPUT_FORMAT(binary)

SECTIONS {
    /*.fill : { . = 0x8000; }*/
	.prg 0x8000 : { SHORT(0x0201) } /*FIXME these are dummy bytes, find a way to not use them*/
    INCLUDE c_sections.ld
    .vectors 0xFFFA : { SHORT(_start) SHORT(_start) SHORT(_start) }
}

/* Provide 256 imaginary (zero page) registers. */
INCLUDE imag_reg/255.ld
ASSERT(__rc0 == 0x0000, "Inconsistent zero page map.")
ASSERT(__rc255 == 0x00ff, "Inconsistent zero page map.")

/* Set initial soft stack address to just above last memory address. (It grows down.) */
__stack = 0xFFF0;
