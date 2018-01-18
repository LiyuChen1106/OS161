#ifndef _VM_H_
#define _VM_H_

#include <machine/vm.h>

/*
 * VM system-related definitions.
 *
 * You'll probably want to add stuff here.
 */

/*
 * Total physical page number
 */
unsigned long page_num;

/* 
 * Record number of free physical pages in coremap 
 */
unsigned long free_ppages;


/* 
 * Variables for page swapping 
 */

// stores information about the disk we are swapping page to and from
struct vnode * disk_file;

// total number of page on the disk
int disk_page_num;

// bit map to record if the page on the disk is swapped or not
struct bitmap* swap_map;


#define FIRST_LEVEL_VPN      0xffc00000
#define SECOND_LEVEL_VPN     0x003ff000
#define OFFSET               0x00000fff

/* Fault-type arguments to vm_fault() */
#define VM_FAULT_READ        0    /* A read was attempted */ //-> valid bit check
#define VM_FAULT_WRITE       1    /* A write was attempted */ //-> valid bit check
#define VM_FAULT_READONLY    2    /* A write to a readonly page was attempted */ //-> dirty bit check

/* Region-type arguments to check_vaddr() */
#define REGION_1 1
#define REGION_2 2
#define REGION_STACK 3
#define REGION_HEAP 4

/* Coremap entry structure */
struct coremap_entry {
    // the virtual addr and its addrspace that this physical page is mapped to
    // may want to change the way of storing as when implementing copy-on-write
    struct addrspace *as;
    vaddr_t vaddr;
    paddr_t paddr;
    paddr_t startpaddr;

    // the page state 
    /*
     * 0: fixed by kernel
     * 1: free
     * 2: allocated by user/dirty
     * 3: clean -> needed?
     */
    int ppage_state;

    // the chunck size
    int chunck_size;

    // number of vadddr mapped to this ppage
    int link;

    // other info must be added when implementing the swapping algo
    //int swapped;//??????????????
};

/* Coremap: 
 * keep track of all the physical pages
 */
struct coremap_entry *coremap;

struct lock *coremap_lock;

/* Initialization function */
void vm_bootstrap(void);

// get pages
paddr_t getppages(unsigned long npages, vaddr_t vaddr, int is_kernel, struct addrspace *as);

/* Allocate/free kernel heap pages (called by kmalloc/kfree) */
vaddr_t alloc_kpages(int npages);
void free_kpages(vaddr_t addr);

/* Allocate/free user heap pages (called by sbrk) */
paddr_t alloc_upages(vaddr_t addr, struct addrspace *as);
void free_upages(paddr_t addr);

/* Fault handling function called by trap code */
int vm_fault(int faulttype, vaddr_t faultaddress);

/* Handle the faultaddress that has been passed through vm_fault */
paddr_t page_fault_handler(vaddr_t faultaddress, 
		struct addrspace *as, int permission);
int load_page(vaddr_t faultaddress, 
		struct addrspace *as, int region, int permission);
int check_vaddr(int faulttype, vaddr_t faultaddress, 
		struct addrspace *as, int region, int permission);


/* Functions for swapping page in and out*/
void swap_in_from_disk(vaddr_t faultaddress, struct addrspace *as);
void swap_out_to_disk();

/* Functions for getting more pages when free-ppages is 0*/
unsigned long getppages_k_swap(unsigned long npages);

void print_coremap();

#endif /* _VM_H_ */
