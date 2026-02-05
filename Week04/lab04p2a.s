// file: lab04p2a.s
// desc: lab04 p1b
// author: @HudsonTran1

.global main
main:

.section .text


    push {r4,r5,r6,r7}

    ldr r4, =W // store address of label W in register 4

    ldr r5, [r4] // store data from the address in register 4 in register 5

    all_done: nop

    pop {r4, r5, r6, r7}

        bx lr

    .section .data

        .word 0xbbbbbbbb

W:      .word 4277009102

        .word 0xeeeeeeee

    .end