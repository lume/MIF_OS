# Virtual machine document

# Outline
- Memory Paging
- MMU and TLB

# Memory Paging

PAGESIZE: 4 K
SWAP -> 1 MB

Virtual memory size: 64 K + DISK

LRU Algorithm for paging:
- Page which has not been used for the longest time in main memory is the one which will be selected for replacement.
- Easy to implement, keep a list, replace pages by looking back into time.

Analog algorithms:
FIFO, LFU


*RAM memory is split into pages of PAGESIZE each. If all memory is used, currently unused memory pages are swapped into disk*

# MMU

MMU will contain TLB for virtual page reference caching and page walker to look for needed pages in memory.

TLB contains two entries.




