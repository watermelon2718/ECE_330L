// Andy Lopez-Martinez

.global _start
_start:

    .func main
    push {r4,r5,r6,r7,lr}

    mov r4, #7 // Moves contents of R7 to R4
    mov r5, #13 // Moves contents of R13 to R5
    subs r6, r4, r5 // r6=r4-r5 without affecting status register flag


    all_done:

    pop {r4,r5,r6,r7,lr}

    bx lr

    .endfunc

    .end
