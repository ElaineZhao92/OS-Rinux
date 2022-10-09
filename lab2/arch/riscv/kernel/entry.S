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
    sd s1, -18*8(sp)
    sd s2, -19*8(sp)
    sd s3, -20*8(sp)
    sd s4, -21*8(sp)
    sd s5, -22*8(sp)
    sd s6, -23*8(sp)
    sd s7, -24*8(sp)
    sd s8, -25*8(sp)
    sd s9, -26*8(sp)
    sd s10,-27*8(sp)
    sd s11,-28*8(sp)
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
    ld s1, -18*8(sp)
    ld s2, -19*8(sp)
    ld s3, -20*8(sp)
    ld s4, -21*8(sp)
    ld s5, -22*8(sp)
    ld s6, -23*8(sp)
    ld s7, -24*8(sp)
    ld s8, -25*8(sp)
    ld s9, -26*8(sp)
    ld s10,-27*8(sp)
    ld s11,-28*8(sp)
    ld t4, -29*8(sp)
    ld t5, -30*8(sp)
    ld t6, -31*8(sp)
    ld sp, -2*8(sp)
    # -----------
    # 4. return from trap
    sret
    # -----------
