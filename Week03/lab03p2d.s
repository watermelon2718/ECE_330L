//Ellyn Tran

.global _start
_start:
.func main
push {r4,r5,r6,r7,lr}
ldr r4, =0x1111 // loads 12 into r4
lsl r4, #3 //left shifts the value of r4 by 3 bits and stores in r4; equivalent to multiplying r4 by 8
lsr r4, #2 //right shifts the value of r4 by 2 bits and stores in r4; equivalent to dividing r4 by 4
all_done:
pop {r4,r5,r6,r7,lr}
bx lr
.endfunc
.end