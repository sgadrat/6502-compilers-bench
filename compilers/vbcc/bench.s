;
; gcc zeropage footprint
;

.exportzp sp
.exportzp r0,   r1,  r2,  r3,  r4,  r5,  r6,  r7,  r8,  r9
.exportzp r10, r11, r12, r13, r14, r15, r16, r17, r18, r19
.exportzp r20, r21, r22, r23, r24, r25, r26, r27, r28, r29
.exportzp r30, r31
.exportzp btmp0, btmp1, btmp2, btmp3

.segment "ZEROPAGE"
sp: .res 2
r0: .res 1
r1: .res 1
r2: .res 1
r3: .res 1
r4: .res 1
r5: .res 1
r6: .res 1
r7: .res 1
r8: .res 1
r9: .res 1
r10: .res 1
r11: .res 1
r12: .res 1
r13: .res 1
r14: .res 1
r15: .res 1
r16: .res 1
r17: .res 1
r18: .res 1
r19: .res 1
r20: .res 1
r21: .res 1
r22: .res 1
r23: .res 1
r24: .res 1
r25: .res 1
r26: .res 1
r27: .res 1
r28: .res 1
r29: .res 1
r30: .res 1
r31: .res 1
btmp0: .res 1
btmp1: .res 1
btmp2: .res 1
btmp3: .res 1

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
