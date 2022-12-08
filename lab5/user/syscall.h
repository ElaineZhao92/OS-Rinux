#define SYS_WRITE   64
#define SYS_GETPID  172
#pragma once
typedef unsigned long uint64_t;

struct pt_regs{
    uint64_t x[32];
    uint64_t sepc, sstatus;
};
void syscall(struct pt_regs* regs);