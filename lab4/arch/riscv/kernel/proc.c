#include "proc.h"
#include "defs.h"
#include "printk.h"
#include "mm.h"
#include "rand.h"

//arch/riscv/kernel/proc.c

extern void __dummy();
extern void __switch_to(struct task_struct* prev, struct task_struct* next);


struct task_struct* idle;           // idle process
struct task_struct* current;        // 指向当前运行线程的 `task_struct`
struct task_struct* task[NR_TASKS]; // 线程数组, 所有的线程都保存在此


void task_init() {
    // 1. 调用 kalloc() 为 idle 分配一个物理页
    // 2. 设置 state 为 TASK_RUNNING;
    // 3. 由于 idle 不参与调度 可以将其 counter / priority 设置为 0
    // 4. 设置 idle 的 pid 为 0
    // 5. 将 current 和 task[0] 指向 idle

    /* YOUR CODE HERE */
    idle = (struct task_struct *)kalloc();
    idle->state = TASK_RUNNING;
    idle->counter = 0;
    idle->priority = 0;
    idle->pid = 0;
    current = idle;
    task[0] = idle;

    // 1. 参考 idle 的设置, 为 task[1] ~ task[NR_TASKS - 1] 进行初始化
    // 2. 其中每个线程的 state 为 TASK_RUNNING, counter 为 0, priority 使用 rand() 来设置, pid 为该线程在线程数组中的下标。
    // 3. 为 task[1] ~ task[NR_TASKS - 1] 设置 `thread_struct` 中的 `ra` 和 `sp`,
    // 4. 其中 `ra` 设置为 __dummy （见 4.3.2）的地址,  `sp` 设置为 该线程申请的物理页的高地址

    /* YOUR CODE HERE */
    for(int i = 1; i < NR_TASKS; i++){
        task[i] = (struct task_struct *)kalloc();
        task[i]-> state = TASK_RUNNING;
        task[i]-> counter = 0;
        task[i]-> priority = rand();
        task[i]-> pid = i;
        task[i]-> thread.ra = (uint64)__dummy;
        task[i]-> thread.sp = (uint64)task[i] + PGSIZE;
    }

    printk("...proc_init done!\n");
}

void dummy() {
    uint64 MOD = 1000000007;
    uint64 auto_inc_local_var = 0;
    int last_counter = -1;
    while(1) {
        if (last_counter == -1 || current->counter != last_counter) {
            last_counter = current->counter;
            auto_inc_local_var = (auto_inc_local_var + 1) % MOD;
            // printk("[PID = %d] is running. auto_inc_local_var = %d\n", current->pid, auto_inc_local_var);
            printk("[PID = %d] is running at address %lx\n", current->pid, current);
        }
    }
}

void do_timer(void) {
    // 1. 如果当前线程是 idle 线程 直接进行调度
    // 2. 如果当前线程不是 idle 对当前线程的运行剩余时间减1 若剩余时间仍然大于0 则直接返回 否则进行调度

    /* YOUR CODE HERE */
    if(current->pid == idle->pid)
        schedule();
    else{
        current -> counter --;
        if(current->counter == 0)
            schedule();
    }
}

void switch_to(struct task_struct* next) {
    /* YOUR CODE HERE */
    if(current->pid != next->pid){
        struct task_struct* prev = current;
        current = next;
        printk("\nswitch to [PID = %d COUNTER = %d PRIORITY = %d]\n", next->pid, next->counter, next->priority);
        __switch_to(prev, next);
    }
}

#ifdef SJF
void schedule(void) {
    /* YOUR CODE HERE */
    int min = 1;
    int flag = 1;//指示是否需要线程是否全部都进行完了
    for(int i = 1; i < NR_TASKS; i++){
        if(task[i]->state == TASK_RUNNING && task[i]->counter != 0){
            flag = 0;
        }
    }
    if(flag){//重新rand
        // printk("\n");
        for(int i = 1; i < NR_TASKS; i ++){
            task[i] -> counter = rand();
            // printk("SET [PID = %d COUNTER = %d]\n", task[i]->pid, task[i]->counter);
        }
        for(int i = 2; i < NR_TASKS; i++){
            if(task[i]->counter < task[min]->counter)
                min = i;
        }
    }else{
        min = 1;
        //找到第一个非零的下标
        for(int i = 1; i < NR_TASKS; i++){
            if(task[i]->counter != 0){
                min = i;
                break;
            }
        }
        for(int i = min + 1; i < NR_TASKS; i++){
            if(task[i] -> counter != 0 && task[i]->counter < task[min]->counter)
                min = i;
        }
    }
    switch_to(task[min]);
}


#else
//Priority
void schedule(void) {
    /* YOUR CODE HERE */
    int next = 0;
    while (1){
        int c = 0;
        int i = NR_TASKS;
        struct task_struct **p = &task[NR_TASKS];
        while(--i){
            if(!*--p)
                continue;
            //find max priority,index is next
            if((*p)->state == TASK_RUNNING && (*p)->counter != 0 && (*p)->priority > c ){
                c = (*p)->priority;
                next = i;
            }
        }
        if(c)   break;
        //printk("\n");
        for(p = &task[1]; p < &task[NR_TASKS]; p++){
            if(*p){
                (*p)->counter = ((*p)->counter >> 1) + (*p) -> priority;
                // printk("SET [PID = %d PRIORITY = %d COUNTER = %d\n", p-&task[0], (*p)->priority, (*p)->counter);
            }
        }
    }
    switch_to(task[next]);
}


#endif
