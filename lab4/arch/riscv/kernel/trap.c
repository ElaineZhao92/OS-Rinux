#include"printk.h"
#include"clock.h"
#include"types.h"

void trap_handler(uint64 scause, uint64 sepc) {
    // 通过 `scause` 判断trap类型
    // 如果是interrupt 判断是否是timer interrupt
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.5 节
    // 其他interrupt / exception 可以直接忽略

    // YOUR CODE HERE
    // printk("kernel is running!\n");
    // Exception code == 5 <- supervisor time interrupt
    if(scause >> 63 == 1 && (scause & 0b11111) == 5){
        //trap && time interrupt
        //printk("[S] Supervisor Mode Timer Interrupt\n");
        do_timer();
        clock_set_next_event();
    }
}
