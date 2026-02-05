//commented by @HudsonTran1
.global _start
_start:
    .func main
    push {r4,r5,r6,r7,lr}

    ldr r4, =68 // stores #68 in register r4
    ldr r5, =45 // stores #45 in register r5
    add r2, r4, r5 // adds the contents of registers r4 and r5, stores the result in register r2

    all_done:

    pop {r4,r5,r6,r7,lr}

    bx lr

    .endfunc

    .end