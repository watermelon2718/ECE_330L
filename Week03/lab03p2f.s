//Ellyn Tran

.global _start
_start:
.func main
push {r4,r5,r6,r7,lr}
ldr r4, =0xFFFFFFFF // loads FFFFFFFF (hex) into r4
ldr r5, =0x12345678 // loads 12345678 (hex) into r5
orr r6, r4, r5 // performs bitwise or between r4 and r5, stores the result in r6
all_done:
pop {r4,r5,r6,r7,lr}
bx lr
.endfunc
.end