#pragma once

#define RAM_SIZE 100
#define VRAM_SIZE 200
#define PAGETABLE_SIZE  10 // 10 pages 20b each 
#define FRAMETABLE_SIZE 5  // 5 frames of physical memory


#define DISK_NAME "devDrv.txt"
#define DISK_DIRECTORY "swapdisk/"
#define DISK_SIZE 100
#define PAGE_SIZE 20
#define SECTOR_SIZE 20
#define SECTOR_COUNT DISK_SIZE / SECTOR_SIZE
#define CHAR_BUFFER_SIZE 4096 // amount of characters that can be displayed
//sectors are fixed size: 512 entries (or bytes)