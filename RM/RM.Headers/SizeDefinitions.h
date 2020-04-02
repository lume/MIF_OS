#pragma once

#define RAM_SIZE 1048576
#define VRAM_SIZE 2097152
#define PAGETABLE_SIZE 512 // 512 pages 4kb each 
#define FRAMETABLE_SIZE 256 // 256 frames of physical memory


#define DISK_NAME "devDrv.txt"
#define DISK_DIRECTORY "swapdisk/"
#define DISK_SIZE 1048576
#define PAGE_SIZE 4096
#define SECTOR_SIZE 4096
#define SECTOR_COUNT 1048576 / SECTOR_SIZE
#define CHAR_BUFFER_SIZE 4096 // amount of characters that can be displayed
