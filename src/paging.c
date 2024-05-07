#include "paging.h"
#include "page_splitting.h"
#include "swapops.h"
#include "stats.h"

 /* The frame table pointer. You will set this up in system_init. */
fte_t *frame_table;


void system_init(void) {
    /*
     * 1. Set the frame table pointer to point to the first frame in physical
     * memory. Zero out the memory used by the frame table.
     *
     * Address "0" in memory will be used for the frame table. This table will
     * contain n frame table entries (fte_t), where n is the number of
     * frames in memory. The frame table will be useful later if we need to
     * evict pages during page faults.
     */
    frame_table = (fte_t *)malloc(NUM_FRAMES * sizeof(fte_t));
    memset(frame_table, 0, NUM_FRAMES);
    frame_table[0].protected = 1;


    /*
     * 2. Mark the first frame table entry as protected.
     *
     * The frame table contains entries for all of physical memory,
     * however, there are some frames we never want to evict.
     * We mark these special pages as "protected" to indicate this.
     */

}

void proc_init(pcb_t *proc) {
    /*
     * 1. Call the free frame allocator (free_frame) to return a free frame for
     * this process's page table. You should zero-out the memory.
     */
    pfn_t freeFrame = free_frame(); //free frame
    proc->saved_ptbr = freeFrame; //set saved_ptbr to free frame

    uint8_t *addr = mem + (freeFrame * PAGE_SIZE); //get address from frame
    for (int i = 0; i < PAGE_SIZE; ++i) { //zero memory
        addr[i] = 0;
    }

    frame_table[freeFrame].protected = 1; //set frame is protected


    /*
     * 2. Update the process's PCB with the frame number
     * of the newly allocated page table.
     *
     * Additionally, mark the frame's frame table entry as protected. You do not
     * want your page table to be accidentally evicted.
     */

}

void context_switch(pcb_t *proc) {
    PTBR = proc->saved_ptbr;
}

uint8_t mem_access(vaddr_t address, char rw, uint8_t data) {


    /* Split the address and find the page table entry */


    /* If an entry is invalid, just page fault to allocate a page for the page table. */


    /* Set the "referenced" bit to reduce the page's likelihood of eviction */


    /*
        The physical address will be constructed like this:
        -------------------------------------
        |     PFN    |      Offset          |
        -------------------------------------
        where PFN is the value stored in the page table entry.
        We need to calculate the number of bits are in the offset.

        Create the physical address using your offset and the page
        table entry.
    */


    /* Either read or write the data to the physical address
       depending on 'rw' */












    /*
    used to read or write data from or to virtual memory
    if page corresponding to virtual addy isn't present in the memory, then page fault
    */
    vpn_t vpn = vaddr_vpn(address); // split address into vpn and offset
    pte_t *ptentry = (pte_t *)(mem +(PAGE_SIZE* PTBR)+(vpn*sizeof(pte_t))); //get pte
    if (!ptentry->valid) { //if not valid, page fault
        page_fault(address);
    }
    uint8_t *storeAddr = mem + (ptentry->pfn *PAGE_SIZE) +vaddr_offset(address); //get physical address
    if(rw == 'r') { //if read, increment reads
        stats.reads++;
        return *storeAddr;
    } else { //if not, then write
        stats.writes++;
        ptentry->dirty = 1;
        *storeAddr = data;
        return data;
    }
}










/*  --------------------------------- PROBLEM 8 --------------------------------------
    When a process exits, you need to free any pages previously occupied by the
    process. Otherwise, every time you closed and re-opened Microsoft Word, it
    would gradually eat more and more of your computer's usable memory.

    To free a process, you must clear the "mapped" bit on every page the process
    has mapped. If the process has swapped any pages to disk, you must call
    swap_free() using the page table entry pointer as a parameter.

    You must also clear the "protected" bits for the page table itself.
    -----------------------------------------------------------------------------------
*/





/*
handles cleanup, frees pages, clears mapped bit, clears protected bit
*/
void proc_cleanup(pcb_t *proc) {
    
    pte_t *ptentry = (pte_t *)(mem +(proc->saved_ptbr*PAGE_SIZE)); //get pte
    for (int i = 0; i < NUM_PAGES; i++, ptentry++) { //loop all pages
        if (ptentry->valid) { //if valid, clear mapped bit
            frame_table[ptentry->pfn].mapped = 0;
        }
        if (ptentry->swap) { //if swapped, free swap
            swap_free(ptentry);
            ptentry->swap = 0;
        }
    }
    frame_table[proc->saved_ptbr].protected = 0; //clear protected bit
}




