;
; gcc zeropage footprint
;

.exportzp tmpfield1, tmpfield2, tmpfield3, tmpfield4, tmpfield5
.exportzp tmpfield6, tmpfield7

.segment "ZEROPAGE"
tmpfield1: .res 1
tmpfield2: .res 1
tmpfield3: .res 1
tmpfield4: .res 1
tmpfield5: .res 1
tmpfield6: .res 1
tmpfield7: .res 1

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

.import benched_routine

nmi:
reset:
irq:
	; Call benched subroutine
	jsr benched_routine

	; Signal the emulator to exit
	lda #0
	sta $5000

	jmp irq
