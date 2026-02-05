.global _start
_start:
	
	.global main
main:

.section  .text
  

    push {r4,r5,r6,r7}

      ldr   r4,=DSECT
      ldrb  r5,[r4,#(A-DSECT)] 	// r5 = A (byte)
      ldrh  r6,[r4,#(B-DSECT)] 	// r6 = B (halfword)
      add   r7,r5,r6 			// r7 = A + B
      str   r7,[r4,#(C-DSECT)]	// C = A + B

    all_done: nop

    pop {r4,r5,r6,r7}
    
      bx lr

      
  .section .data

  .org 234
  .align 2,0xa5
  
DSECT:
     .word 0xbbbbbbbb

A:   .byte 123		// 0x7b

     .align 1,0xaa
B:   .short 47587 	// 0xb9e3

     .align 2,0xa5
C:   .word ~0

     .align 3,0xa5	// will become 0x0000BA5E
     .word 0xeeeeeeee

  .end
