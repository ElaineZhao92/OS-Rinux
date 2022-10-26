    .section .text.entry
    .align 2
    .globl _traps 
_traps:
    # YOUR CODE HERE
    # -----------
    # 1. save 32 registers and sepc to stack
    sd ra, -1*8(sp)
    sd sp, -2*8(sp)
    sd gp, -3*8(sp)
    sd tp, -4*8(sp)
    sd t0, -5*8(sp)
    sd t1, -6*8(sp)
    sd t2, -7*8(sp)
    sd s0, -8*8(sp)
    sd s1, -9*8(sp)
    sd a0, -10*8(sp)
    sd a1, -11*8(sp)
    sd a2, -12*8(sp)
    sd a3, -13*8(sp)
    sd a4, -14*8(sp)
    sd a5, -15*8(sp)
    sd a6, -16*8(sp)
    sd a7, -17*8(sp)
    sd s2, -18*8(sp)
    sd s3, -19*8(sp)
    sd s4, -20*8(sp)
    sd s5, -21*8(sp)
    sd s6, -22*8(sp)
    sd s7, -23*8(sp)
    sd s8, -24*8(sp)
    sd s9, -25*8(sp)
    sd s10,-26*8(sp)
    sd s11,-27*8(sp)
    sd t3, -28*8(sp)
    sd t4, -29*8(sp)
    sd t5, -30*8(sp)
    sd t6, -31*8(sp)
    ##save sepc
    csrr t0, sepc
    sd t0, -32*8(sp)
    addi sp, sp, -256
    # -----------
    # 2. call trap_handler
    csrr a0, scause
    csrr a1, sepc
    call trap_handler
    # -----------
    # 3. restore sepc and 32 registers (x2(sp) should be restore last) from stack
    ld t0, 0(sp)
    csrw sepc, t0
    addi sp, sp, 256
    ld ra, -1*8(sp)
    ld gp, -3*8(sp)
    ld tp, -4*8(sp)
    ld t0, -5*8(sp)
    ld t1, -6*8(sp)
    ld t2, -7*8(sp)
    ld s0, -8*8(sp)
    ld s1, -9*8(sp)
    ld a0, -10*8(sp)
    ld a1, -11*8(sp)
    ld a2, -12*8(sp)
    ld a3, -13*8(sp)
    ld a4, -14*8(sp)
    ld a5, -15*8(sp)
    ld a6, -16*8(sp)
    ld a7, -17*8(sp)
    ld s2, -18*8(sp)
    ld s3, -19*8(sp)
    ld s4, -20*8(sp)
    ld s5, -21*8(sp)
    ld s6, -22*8(sp)
    ld s7, -23*8(sp)
    ld s8, -24*8(sp)
    ld s9, -25*8(sp)
    ld s10,-26*8(sp)
    ld s11,-27*8(sp)
    ld t3, -28*8(sp)
    ld t4, -29*8(sp)
    ld t5, -30*8(sp)
    ld t6, -31*8(sp)
    ld sp, -2*8(sp)
    # -----------
    # 4. return from trap
    sret
    # -----------

    .globl __dummy
__dummy:
    # YOUR CODE HERE
    la s0, dummy
    csrw sepc, s0
    sret


    .globl __switch_to
__switch_to:
    # save state to prev process
    # YOUR CODE HERE
    sd ra, 5*8(a0)
    sd sp, 6*8(a0)
    sd s0, 7*8(a0)
    sd s1, 8*8(a0)
    sd s2, 9*8(a0)
    sd s3, 10*8(a0)
    sd s4, 11*8(a0)
    sd s5, 12*8(a0)
    sd s6, 13*8(a0)
    sd s7, 14*8(a0)
    sd s8, 15*8(a0)
    sd s9, 16*8(a0)
    sd s10, 17*8(a0)
    sd s11, 18*8(a0)
    # restore state from next process
    # YOUR CODE HERE
    ld ra, 5*8(a1)
    ld sp, 6*8(a1)
    ld s0, 7*8(a1)
    ld s1, 8*8(a1)
    ld s2, 9*8(a1)
    ld s3, 10*8(a1)
    ld s4, 11*8(a1)
    ld s5, 12*8(a1)
    ld s6, 13*8(a1)
    ld s7, 14*8(a1)
    ld s8, 15*8(a1)
    ld s9, 16*8(a1)
    ld s10, 17*8(a1)
    ld s11, 18*8(a1)
    
    ret