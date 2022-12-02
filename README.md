# Operating-System
Course of ZJU in 22秋冬

### lab0 GDB & QEMU 调试 64 位 RISC-V LINUX
1. 使用交叉编译工具, 完成Linux内核代码编译
2. 使用QEMU运行内核
3. 熟悉GDB和QEMU联合调试
### lab1 RV64 内核引导
1. 学习 RISC-V 汇编， 编写 head.S 实现跳转到内核运行的第一个 C 函数。
2. 学习 OpenSBI，理解 OpenSBI 在实验中所起到的作用，并调用 OpenSBI 提供的接口完成字符的输出。
3. 学习 Makefile 相关知识， 补充项目中的 Makefile 文件， 来完成对整个工程的管理
### lab2 RV64 时钟中断处理
1. 学习 RISC-V 的 trap 处理相关寄存器与指令，完成对 trap 处理的初始化。
2. 理解 CPU 上下文切换机制，并正确实现上下文切换功能。
3. 编写 trap 处理函数，完成对特定 trap 的处理。
4. 调用 OpenSBI 提供的接口，完成对时钟中断事件的设置。
### lab3 RV64 内核线程调度
1. 了解线程概念, 并学习线程相关结构体, 并实现线程的初始化功能。
2. 了解如何使用时钟中断来实现线程的调度。
3. 了解线程切换原理, 并实现线程的切换。
4. 掌握简单的线程调度算法, 并完成两种简单调度算法的实现。
### lab4 RV64 虚拟内存管理
1. 学习虚拟内存的相关知识，实现物理地址到虚拟地址的切换。
2. 了解 RISC-V 架构中 SV39 分页模式，实现虚拟地址到物理地址的映射，并对不同的段进行相应的权限设置。
### lab5 RV64 

### lab6 RV64 用户态程序（bonus）
1. 创建用户态进程，并设置 sstatus 来完成内核态转换至用户态。
2. 正确设置用户进程的用户态栈和内核态栈， 并在异常处理时正确切换。
3. 补充异常处理逻辑，完成指定的系统调用（ SYS_WRITE, SYS_GETPID ）功能。
这部分实验在做的过程中，把之前实验的bug全都暴露出来了...做着做着报错了，把可能的地方都想了一遍结果全都不是。晕啦！
