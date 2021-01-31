.export benched_routine
.segment "CODE"
benched_routine:
	ldx #200
	copy_one_byte:
		lda $400, x
		sta $200, x
		dex
		bne copy_one_byte
	rts
