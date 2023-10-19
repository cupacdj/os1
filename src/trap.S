.globl trap

trap:
    j interrupts
    j timer
    .skip 28
    j console

interrupts:
    addi sp, sp, -8
    sd a0, (sp)
    csrr a0, sepc
    addi a0,a0,4
    csrw sepc, a0
    ld a0, (sp)
    addi sp, sp, 8
    sret
timer:
    sret
console:
    sret