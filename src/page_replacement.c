#include "types.h"
#include "pagesim.h"
#include "paging.h"
#include "swapops.h"
#include "stats.h"
#include "util.h"

pfn_t select_victim_frame(void);


/**
 * selects victim frame that will be freed and then returned
 * if current mapped and is dirty, then write to swap
*/
pfn_t free_frame(void) {
    //selects frame to free
    pfn_t victim_frame = select_victim_frame();
    //gets pointer to correspeonding ftentry
    fte_t *ftentry = &frame_table[victim_frame];
    //if framme mapped, then get a pointer to pte then check if modified then increment writebacks
    if (ftentry->mapped) {
        pte_t *ptentry=(pte_t*)(mem+(PAGE_SIZE*ftentry->process->saved_ptbr)+(ftentry->vpn * sizeof(pte_t)));
        if (ptentry->dirty) {
            stats.writebacks++;
            swap_write(ptentry, mem + (victim_frame * PAGE_SIZE));
        }
        ptentry->valid = 0; //pte not valid
        ftentry->mapped = 0; //fte not mapped
    }
    return victim_frame;
}













    /*
     * If victim frame is currently mapped:
     *
     * 1) Look up the corresponding page table entry
     * 2) If the entry is dirty, write it to disk with swap_write()
     * 3) Mark the original page table entry as invalid
     */

    /* If the victim is in use, we must evict it first */


    /* Return the pfn */
    




pfn_t select_victim_frame() {
    /* See if there are any free frames first */
    size_t num_entries = MEM_SIZE / PAGE_SIZE;
    for (size_t i = 0; i < num_entries; i++) {
        if (!frame_table[i].protected && !frame_table[i].mapped) {
            return i;
        }
    }

    if (replacement == RANDOM) {
        /* Play Russian Roulette to decide which frame to evict */
        pfn_t last_unprotected = NUM_FRAMES;
        for (pfn_t i = 0; i < num_entries; i++) {
            if (!frame_table[i].protected) {
                last_unprotected = i;
                if (prng_rand() % 2) {
                    return i;
                }
            }
        }
        /* If no victim found yet take the last unprotected frame
           seen */
        if (last_unprotected < NUM_FRAMES) {
            return last_unprotected;
        }
    } 

    /* If every frame is protected, give up. This should never happen
       on the traces we provide you. */
    panic("System ran out of memory\n");
    exit(1);
}
