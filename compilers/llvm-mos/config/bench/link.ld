/* 6502 simulator linker script.
 *
 * Produces a 64KiB memory image file usable with the provided simulator.
 */

OUTPUT_FORMAT(binary)

SECTIONS {
	/*.fill : { . = 0x8000; }*/
	/*.data : { *(.data .data.*) }*/ /*FIXME don't know how to state "put it after rodata in generated file, it is placed at 0x200 at runtime" */
	.bss 0x0200 (NOLOAD) : { *(.bss .bss.* COMMON) }
	.noinit (NOLOAD) : { *(.noinit .noinit.*) }

	.text 0x8000 : { *(.start) *(.text .text.*) }
	.rodata : { *(.rodata .rodata.*) }

	INCLUDE c_sections.ld
	.vectors 0xFFFA : { SHORT(_start) SHORT(_start) SHORT(_start) }
}

/* Provide 256 imaginary (zero page) registers. */
INCLUDE imag_reg/255.ld
ASSERT(__rc0 == 0x0000, "Inconsistent zero page map.")
ASSERT(__rc255 == 0x00ff, "Inconsistent zero page map.")

/* Set initial soft stack address. (It grows down.) */
__stack = 0x07ff;
