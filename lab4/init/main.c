#include "printk.h"
#include "sbi.h"

extern void test();

int start_kernel() {
    // puti(2021);
    // puts(" Hello RISC-V\n");
    
    // puti(csr_read(sstatus));
    // puts(" \n");
    // csr_write(sscratch,100);
    // puti(csr_read(sscratch));
 
    test(); // DO NOT DELETE !!!

	return 0;
}
