;
; gcc zeropage footprint
;

.exportzp _sp0, _sp1, _fp0, _fp1
.exportzp _r0, _r1, _r2, _r3, _r4, _r5, _r6, _r7
.exportzp _s0, _s1, _s2, _s3, _s4, _s5, _s6, _s7
.exportzp _e0, _e1, _e2, _e3, _e4, _e5, _e6, _e7
.exportzp _e8, _e9, _e10, _e11, _e12, _e13, _e14, _e15
.exportzp _e16, _e17, _e18, _e19, _e20, _e21, _e22, _e23
.exportzp _e24, _e25, _e26, _e27, _e28, _e29, _e30, _e31
.exportzp _tmp0, _tmp1
.exportzp _sa, _sx, _sy

.segment "ZEROPAGE"
_sp0: .res 1
_sp1: .res 1
_fp0: .res 1
_fp1: .res 1
_r0: .res 1
_r1: .res 1
_r2: .res 1
_r3: .res 1
_r4: .res 1
_r5: .res 1
_r6: .res 1
_r7: .res 1
_s0: .res 1
_s1: .res 1
_s2: .res 1
_s3: .res 1
_s4: .res 1
_s5: .res 1
_s6: .res 1
_s7: .res 1
_e0: .res 1
_e1: .res 1
_e2: .res 1
_e3: .res 1
_e4: .res 1
_e5: .res 1
_e6: .res 1
_e7: .res 1
_e8: .res 1
_e9: .res 1
_e10: .res 1
_e11: .res 1
_e12: .res 1
_e13: .res 1
_e14: .res 1
_e15: .res 1
_e16: .res 1
_e17: .res 1
_e18: .res 1
_e19: .res 1
_e20: .res 1
_e21: .res 1
_e22: .res 1
_e23: .res 1
_e24: .res 1
_e25: .res 1
_e26: .res 1
_e27: .res 1
_e28: .res 1
_e29: .res 1
_e30: .res 1
_e31: .res 1
_tmp0: .res 1
_tmp1: .res 1
_sa: .res 1
_sx: .res 1
_sy: .res 1

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
	; Initialize C stack
	lda #$ff
	sta _sp0
	lda #$07
	sta _sp1

	; Call benched subroutine
	jsr benched_routine

	; Signal the emulator to exit
	lda #0
	sta $5000

	jmp irq
