//file: lab04p2b.s
//desc: lab04 p2b
//Ellyn Tran

.global main
main:

.section .text

	push {r4, r5, r6, r7}
	
	ldr r4, =DS //load memory location of DS into r4
	ldr r5, [r4, #(B-DS)] //load data at location r4 + (B - DS) into r5
	ldr r6, [r4, #(W-DS)] //load data at location r4 + (W - DS) into r6
	
	all_done: nop
	
	pop {r4, r5, r6, r7}
	
		bx lr
		
	.section .data
	.org 300
	.align 2, 0xaa

		.word 0xbbbbbbbb

B: .byte 85 //0x55

	.align 2, 0xaa
	
DS: .word ~0

	.align 2, 0xaa
	
W: .word 287454020 //0x11223344

	.word 0xeeeeeeee

.end