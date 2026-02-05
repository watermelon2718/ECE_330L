// Andy Lopez-Martinez

.global _start
_start:

    .func main
    push {r4,r5,r6,r7,lr}

    ldr r4, =(1<<31)-1 // Loads 32-bit constant value into register r4
    mov r5, #4 // Moves content of r4 to r5
    adds r2, r4, r5 // Adds values of r4, r5 and puts the results in r2 without any effect on the status register flag

    all_done:

    pop {r4,r5,r6,r7,lr}

    bx lr

    .endfunc

    .end
