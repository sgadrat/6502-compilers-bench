__rc0 = 0x00;
INCLUDE imag-regs.ld
ASSERT(__rc0 == 0x00, "Inconsistent zero page map.")
ASSERT(__rc31 == 0x1f, "Inconsistent zero page map.")

MEMORY {
	zp : ORIGIN = __rc31 + 1, LENGTH = 0x100 - (__rc31 + 1)
	ram (rw) : ORIGIN = 0x0200, LENGTH = 0x0600
	rom (rx) : ORIGIN = 0x8000, LENGTH = 0x8000
}

REGION_ALIAS("c_ram", ram)
REGION_ALIAS("c_rom", rom)

SECTIONS {
	.text : {
		INCLUDE text-sections.ld
	} >c_rom
	.rodata : { INCLUDE rodata-sections.ld } >c_rom
	.data : { INCLUDE data-sections.ld } >c_ram AT>c_rom
	__data_load_start = LOADADDR(.data);
	__data_size = SIZEOF(.data);
	.zp.data : { INCLUDE zp-data-sections.ld } >zp AT>c_rom
	__zp_data_load_start = LOADADDR(.zp.data);
	__zp_data_size = SIZEOF(.zp.data);
	.zp.bss (NOLOAD) : { INCLUDE zp-bss-sections.ld } >zp
	INCLUDE zp-bss-symbols.ld
	INCLUDE zp-noinit.ld
	.bss (NOLOAD) : { INCLUDE bss-sections.ld } >c_ram
	INCLUDE bss-symbols.ld
	.noinit (NOLOAD) : { INCLUDE noinit-sections.ld } >c_ram

	.vector 0xfffa : { SHORT(_start) SHORT(_start) SHORT(_start)} >c_rom
}

OUTPUT_FORMAT {
	FULL(rom)
}
