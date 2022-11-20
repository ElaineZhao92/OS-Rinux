#include "defs.h"
#include "mm.h"
#include "printk.h"

/* virtual address */
/* | VPN[2] 9bit | VPN[1] 9bit | VPN[0] 9bit | page offset 12bit | */
#define VPN2(va) ((va >> 30) & 0x1ff)   // VPN[2] 9 bit
#define VPN1(va) ((va >> 21) & 0x1ff)   // VPN[1] 9 bit
#define VPN0(va) ((va >> 12) & 0x1ff)   // VPN[0] 9 bit

extern char _stext[];
extern char _srodata[];
extern char _sdata[];

/* early_pgtbl: 用于 setup_vm 进行 1GB 的 映射。 */
uint64  early_pgtbl[512] __attribute__((__aligned__(0x1000)));

void setup_vm(void) {
    /* 
    1. 由于是进行 1GB 的映射 这里不需要使用多级页表 
    2. 将 va 的 64bit 作为如下划分： | high bit | 9 bit | 30 bit |
        high bit 可以忽略
        中间9 bit 作为 early_pgtbl 的 index
        低 30 bit 作为 页内偏移 这里注意到 30 = 9 + 9 + 12， 即我们只使用根页表， 根页表的每个 entry 都对应 1GB 的区域。 
    3. Page Table Entry 的权限 V | R | W | X 位设置为 1
    */

    /* 1. 为early_pgtbl 分配一页内存*/
    memset(early_pgtbl, 0x0, PGSIZE);

    uint64 pa = PHY_START, va = PHY_START, addr;
    int index = VPN2(va);
    // 这是因为64位的virtual memory第39-30位是index
    
    /* 2. 直接映射到0x80000000 
       V R W X 都设置成1的话，还需要在末尾4位添上0x1111 
    */
    addr = ((pa >> 30) & 0x3ffff) << 28 | 0xf;
    // printk("index=%d\n", index);
    early_pgtbl[index] = addr;

    /* 3. 线性映射到0xffffffe000000000*/
    va = VM_START;
    index = VPN2(va);
    // printk("index=%d\n", index);
    early_pgtbl[index] = addr;

}

/* 创建多级页表映射关系 */
void create_mapping(uint64 * root_pgtbl, uint64 va, uint64 pa, uint64 sz, int perm) {
    /*              
    root_pgtbl 为根页表的基地址
    va, pa 为需要映射的虚拟地址、物理地址
    sz 为映射的大小
    perm 为映射的读写权限

    创建多级页表的时候可以使用 kalloc() 来获取一页作为页表目录
    可以使用 V bit 来判断页表项是否存在
    */

    unsigned int vpn[3]; // 存储页表中所求PTE的偏移量
    uint64 * pgtbl[3]; // 存储页表的物理地址
    uint64 pte[3]; // 存储页表中所求PTE的值
    uint64 * new_pg;

    uint64 end = va + sz; // end是判断终止地址

    while (va < end){
        vpn[0] = VPN0(va);
        vpn[1] = VPN1(va);
        vpn[2] = VPN2(va);

        pgtbl[2] = root_pgtbl;
        pte[2] = pgtbl[2][vpn[2]];
        // 如果没有这条entry, 目前还是invalid
        if (!(pte[2] & 0x1)) {
            new_pg = (uint64 *)kalloc();
            // 1. 把新page的address拿来 先处理physical和virtual的映射关系，让virtual地址变成physical地址
            // 2. 除掉末尾12位的offset，将其变成pte,pte最后10位为权限位
            // 3. pte设置flags，表示这里指向的是下一pagetable而不是物理地址
            pte[2] = ((((uint64)new_pg - PA2VA_OFFSET) >> 12) << 10);
            pte[2] |= 0x1; //设置flags
            pgtbl[2][vpn[2]] = pte[2];
        }
        // 处理第二级页表
        pgtbl[1] = (uint64 *)((pte[2] >> 10) << 12);
        pte[1] = pgtbl[1][vpn[1]];
        if (!(pte[1] & 0x1)) {
            new_pg = (uint64 *)kalloc();
            pte[1] = ((((uint64)new_pg - PA2VA_OFFSET) >> 12) << 10);
            pte[1] |= 0x1;
            pgtbl[1][vpn[1]] = pte[1];
        }

        pgtbl[0] = (uint64 *)((pte[1] >> 10) << 12);
        pte[0] = ((pa >> 12) << 10) | (perm & 15);
        pgtbl[0][vpn[0]] = pte[0];

        va += PGSIZE;
        pa += PGSIZE;
    }

    return;
}

/* swapper_pg_dir: kernel pagetable 根目录， 在 setup_vm_final 进行映射。 */
uint64  swapper_pg_dir[512] __attribute__((__aligned__(0x1000)));

void setup_vm_final(void) {

    memset(swapper_pg_dir, 0x0, PGSIZE);

    // No OpenSBI mapping required
    // text段长度0x2000
    uint64 size = (uint64)_srodata - (uint64)_stext;
    uint64 va = VM_START + OPENSBI_SIZE;
    uint64 pa = PHY_START + OPENSBI_SIZE;
    // mapping kernel text X|-|R|V
    // 0x1011 = 11
    create_mapping(swapper_pg_dir, va, pa, size, 11);

    // mapping kernel rodata -|-|R|V
    // 0x 0011 = 3
    pa += size;
    va += size;
    // rodata段长度0x1000
    size = (uint64)_sdata - (uint64)_srodata;
    create_mapping(swapper_pg_dir, va, pa, size, 3);

    // mapping other memory -|W|R|V
    // 0x0111 = 7
    pa += size;
    va += size;
    // 其他地址长度
    size = 0x8000000-((uint64)_sdata - (uint64)_stext);
    create_mapping(swapper_pg_dir, va, pa, size, 7);

    // set satp with swapper_pg_dir
    // YOUR CODE HERE
    uint64 PG_DIR = (uint64)swapper_pg_dir - PA2VA_OFFSET;
    // 这部分和relocate中的思路一样，只不过这次是从PG_DIR开始写入
    asm volatile(
        "li t0, 8\n"
        "slli t0, t0, 60\n"
        "mv t1, %[PG_DIR]\n"
        "srli t1, t1, 12\n"
        "add t0, t0, t1\n"
        "csrw satp, t0"
        :
        :[PG_DIR]"r"(PG_DIR)
        :"memory"
    );

    // flush TLB
    asm volatile("sfence.vma zero, zero");

    // flush icache
    asm volatile("fence.i");
    
    return;
}
