#include"printk.h"
#include"clock.h"
#include"types.h"
#include "stdint.h"
#include "../user/syscall.h"

void trap_handler(uint64_t scause, uint64_t sepc, struct pt_regs *regs) {
    // 通过 `scause` 判断trap类型
    // 如果是interrupt 判断是否是timer interrupt
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.5 节
    // 其他interrupt / exception 可以直接忽略

    // YOUR CODE HERE
    // printk("kernel is running!\n");
    // Exception code == 5 <- supervisor time interrupt
    if(scause >> 63 == 1){ // trap
        if((scause & 0b11111) == 5){
        //trap && time interrupt
        //printk("[S] Supervisor Mode Timer Interrupt\n");
            clock_set_next_event();
            do_timer();
        }
    }
    else if(scause >> 63 == 0){
        if((scause & 0b11111) == 8)
            syscall(regs);
    }
}