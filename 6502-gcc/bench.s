;
; iNES header
;

.segment "HEADER"

INES_MAPPER = 0 ; 0 = NROM
INES_MIRROR = 1 ; 0 = horizontal mirroring, 1 = vertical mirroring
INES_SRAM   = 0 ; 1 = battery backed SRAM at $6000-7FFF

.byte 'N', 'E', 'S', $1A ; ID
.byte $02 ; 16k PRG chunk count
.byte $01 ; 8k CHR chunk count
.byte INES_MIRROR | (INES_SRAM << 1) | ((INES_MAPPER & $f) << 4)
.byte (INES_MAPPER & %11110000)
.byte $0, $0, $0, $0, $0, $0, $0, $0 ; padding

;
; CHR ROM
;

.segment "TILES"
; Background
.res 4096, $00
; Sprites
.res 4096, $01

;
; vectors placed at top 6 bytes of memory area
;

.segment "VECTORS"
.word nmi
.word reset
.word irq

;
;
;

.segment "OAM"
.res 256

;
; routines
;

.segment "CODE"

nmi:
reset:
irq:
	jmp irq
