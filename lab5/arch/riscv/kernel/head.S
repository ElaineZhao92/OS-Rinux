#include "defs.h"
.extern start_kernel
.extern setup_vm
.extern setup_vm_final

    .section .text.init
    .globl _start
_start:
    la sp, boot_stack_top
    
    call setup_vm
    call relocate
    call mm_init
    call setup_vm_final
    call task_init
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
    # csrr t0, sstatus ##read in ssatus
    # ori t0, t0, 0x2  ##sie is the second bit
    # csrw sstatus, t0 ##write to sstatus

    # ------------------
    #call task_init
    call start_kernel
    # ------------------
    # - your lab1 code -
    # ------------------

relocate:
    # set ra = ra + PA2VA_OFFSET
    # set sp = sp + PA2VA_OFFSET (If you have set the sp before)
    # -------------------- 
    #   YOUR Lab4 CODE   -
    # -------------------- 
    li t0, PA2VA_OFFSET
    add ra, ra, t0
    add sp, sp, t0
    # set satp with early_pgtbl
    # --------------------  
    #   YOUR Lab4 CODE   -
    # --------------------
    ## 1. 设置mode为8 
    li t0, 8
    slli t0, t0, 60
    ## 2. load PPN 页表起始物理地址
    la t1, early_pgtbl
    ### 32bit -> 44bit
    srli t1, t1, 12
    ## 3. 组成satp寄存器
    add t0, t0, t1
    csrw satp, t0
    
    # flush tlb
    sfence.vma zero, zero
    # flush icache
    fence.i

    ret

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space  4096 #4K

    .globl boot_stack_top
boot_stack_top:
