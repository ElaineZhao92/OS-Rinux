# Operating-System
Course of ZJU in 22秋冬
-  lab0 GDB & QEMU 调试 64 位 RISC-V LINUX
1. 使用交叉编译工具, 完成Linux内核代码编译
2. 使用QEMU运行内核
3. 熟悉GDB和QEMU联合调试
-  lab1 RV64 内核引导
1. 学习 RISC-V 汇编， 编写 head.S 实现跳转到内核运行的第一个 C 函数。
2. 学习 OpenSBI，理解 OpenSBI 在实验中所起到的作用，并调用 OpenSBI 提供的接口完成字符的输出。
3. 学习 Makefile 相关知识， 补充项目中的 Makefile 文件， 来完成对整个工程的管理
-  lab2 RV64 时钟中断处理
1. 学习 RISC-V 的 trap 处理相关寄存器与指令，完成对 trap 处理的初始化。
2. 理解 CPU 上下文切换机制，并正确实现上下文切换功能。
3. 编写 trap 处理函数，完成对特定 trap 的处理。
4. 调用 OpenSBI 提供的接口，完成对时钟中断事件的设置。
