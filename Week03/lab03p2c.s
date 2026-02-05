//Ellyn Tran

.global _start
_start:
.func main
push {r4,r5,r6,r7,lr}
ldr r4, =68 // equivalent to mov r4, #68 instruction; moves #68 into r4
ldr r5, =45 // equivalent to mov r4, #45 instruction; moves #45 into r5
mvn r6, r5 //performs bitwise XOR and moves result into r6 (2's complement)
add r6, #1 // adds 1 to r6, transforming into a 2's complement
add r2, r4, r6 //adds r4 and r6, stores result in r2
all_done:
pop {r4,r5,r6,r7,lr}
bx lr
.endfunc
.end