#include "paging.h"
#include "swapops.h"
#include "stats.h"

void page_fault(vaddr_t address) {
    /* First, split the faulting address and locate the page table entry */


    /* It's a page fault, so the entry obviously won't be valid. Grab
       a frame to use by calling free_frame(). */


    /* Update the page table entry. Make sure you set any relevant bits. */


    /* Update the frame table. Make sure you set any relevant bits. */


    /* Initialize the page's memory. On a page fault, it is not enough
     * just to allocate a new frame. We must load in the old data from
     * disk into the frame. If there was no old data on disk, then
     * we need to clear out the memory (why?).
     *
     * 1) Get a pointer to the new frame in memory.
     * 2) If the page has swap set, then we need to load in data from memory
     *    using swap_read().
     * 3) Else, just clear the memory.
     *
     * Otherwise, zero the page's memory. If the page is later written
     * back, swap_write() will automatically allocate a swap entry.
     */
   stats.page_faults++;

   vpn_t vpn = vaddr_vpn(address);
   pte_t *pte = (pte_t *)(mem + (PAGE_SIZE * PTBR) + (vpn * sizeof(pte_t)));
   pfn_t frame = free_frame();
   uint8_t* frame_base_addr = (uint8_t*)(mem + (frame * PAGE_SIZE));
   pte->dirty = 0;
   pte->valid = 1;
   pte->pfn = frame;
   frame_table[frame].mapped = 1;
   frame_table[frame].process = current_process;
   frame_table[frame].vpn = vpn;
   if (!swap_exists(pte)) {
       for (int i = 0; i < PAGE_SIZE; i++) {
           frame_base_addr[i] = 0;
       }
   } else {
       swap_read(pte, frame_base_addr);
   }
}
