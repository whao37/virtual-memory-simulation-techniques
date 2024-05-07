#include "paging.h"
#include "stats.h"

/* The stats. See the definition in stats.h. */
stats_t stats;

void compute_stats() {
    stats.accesses = stats.writes + stats.reads;
    uint64_t total_memory_read_time = MEMORY_READ_TIME * stats.accesses;
    uint64_t total_disk_write_time = DISK_PAGE_WRITE_TIME * stats.writebacks;
    uint64_t total_disk_read_time = DISK_PAGE_READ_TIME * stats.page_faults;
    uint64_t total_time = total_memory_read_time + total_disk_write_time + total_disk_read_time;

    stats.aat = (double)total_time / stats.accesses;
}
