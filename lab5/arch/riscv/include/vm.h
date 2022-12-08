#include "defs.h"
#include "mm.h"
#include "printk.h"
#include "string.h"

extern unsigned long swapper_pg_dir[512];

void setup_vm(void);
void create_mapping(unsigned long *root_pgtbl, unsigned long va, unsigned long pa, unsigned long sz, int perm);
void setup_vm_final(void);