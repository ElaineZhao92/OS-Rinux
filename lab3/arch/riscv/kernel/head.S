.extern start_kernel

    .section .text.init
    .globl _start
_start:
    # YOUR CODE HERE
    # ------------------
    # set stvec = _traps
    la t0, _traps
    csrw stvec, t0
    # ------------------
    # set sie[STIE] = 1
    csrr t0, sie ##read in sie
    ori t0, t0, 0x20
    csrw sie, t0 ##write to sie
    # ------------------
    # set first time interrupt
    rdtime t0
    li t1, 10000000 ##time interval
    add a0, t0, t1
    mv a6, zero     ## set_timer ecall
    mv a7, zero
    ecall
    # ------------------
    # set sstatus[SIE] = 1
    csrr t0, sstatus ##read in ssatus
    ori t0, t0, 0x2  ##sie is the second bit
    csrw sstatus, t0 ##write to sstatus

    # ------------------
    la sp, boot_stack_top
    call mm_init
    call task_init
    csrsi sstatus, 0x2
    call start_kernel
    # ------------------
    # - your lab1 code -
    # ------------------


    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space  4096 #4K

    .globl boot_stack_top
boot_stack_top:
