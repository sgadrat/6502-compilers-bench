.export benched_routine
.segment "CODE"

source = $00
dest = $02

memcpy:
	copy_one_byte:
		lda (source), y
		sta (dest), y
		dey
		bne copy_one_byte
	rts

benched_routine:
	; Set parameters
	;  - source = $0400
	;  - dest = $0200
	;  - n = 200
	ldy #200
	lda #$00
	sta source
	sta dest
	lda #$04
	sta source+1
	lda #$02
	sta dest+1

	; Call memcpy
	jmp memcpy
	;rst ; useless, jump to a subroutine
