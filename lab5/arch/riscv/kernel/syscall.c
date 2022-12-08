#include "proc.h"
#include "printk.h"
#include "./../../../user/syscall.h"

void syscall(struct pt_regs* regs) {
    uint64_t ecall = regs->x[17];
    if (ecall == SYS_WRITE) {
        sys_write((unsigned int)regs->x[10], (const char *)regs->x[11], (size_t)regs->x[12]);
    }
    else if (ecall == SYS_GETPID) {
        regs->x[10] = current->pid;
    }
}

void sys_write(unsigned int fd, const char* buf, size_t count){
    if (fd == 1) {
        fd = printk((char *)buf);
    }
}