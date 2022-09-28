.extern start_kernel

    .section .text.entry
    .globl _start
_start:
    # ------------------
    # - your code here -
    # set up the stack of 4K
    la sp, boot_stack_top
    call start_kernel
    # jump to start_kernel
    # ------------------

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4 * 1024 # 4K <-- change to your stack size

    .globl boot_stack_top
boot_stack_top: