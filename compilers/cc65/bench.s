;
; gcc zeropage footprint
;

.exportzp   sp, sreg, regsave, regbank
.exportzp   tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4

.segment "ZEROPAGE"

sp: .res 2
sreg: .res 2
regsave: .res 2
regbank: .res 2

tmp1: .res 1
tmp2: .res 1
tmp3: .res 1
tmp4: .res 1
ptr1: .res 2
ptr2: .res 2
ptr3: .res 2
ptr4: .res 2

;
; vectors placed at top 6 bytes of memory area
;

.segment "VECTORS"
.word nmi
.word reset
.word irq

;
; routines
;

.segment "MAIN"

.import _benched_routine

nmi:
reset:
irq:
	; Initialize C stack
	lda #$ff
	sta sp
	lda #$07
	sta sp+1

	; Call benched subroutine
	jsr _benched_routine

	; Signal the emulator to exit
	lda #0
	sta $5000

	jmp irq
