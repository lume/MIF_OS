#pragma once
#include <array>
#include <map>
#include <vector>
// memcontrol is responsible for access to RAM, paging. See this as the MMU

#define RAM_SIZE 1048576
#define VRAM_SIZE 2097152
#define PAGETABLE_SIZE 512 // 512 pages 4kb each 
#define FRAMETABLE_SIZE 256 // 256 frames of physical memory

//TODO: Implement virtual memory here

//pages amd frames are fixed size (4kb)

struct Page 
{
    bool used;
    int timesAccessed;
    int frame; // if frame > 255, we go to a swap in our disk
};

struct Memory
{
    //memory protection is planned to be added at the OS level
    std::vector<int> usedPages;
    std::vector<int> addresses;
};

struct Segment
{
    Memory memory;
    int readPointer;  // Pointer to the piece of segment where last read occured
    int writePointer; // Pointer to the piece of segment where last write occured
    char direction;   // if is set to 1, memory write/read is backward (stack)
};

inline std::array<uint16_t, RAM_SIZE> RAM = {0};
inline std::array<uint16_t, VRAM_SIZE> VRAM = {0};
inline std::array<Page, PAGETABLE_SIZE> pageTable;
inline std::array<int, FRAMETABLE_SIZE> frameTable;

class Memcontrol
{
    public: 
        Memcontrol();

        Memory AllocateMemory(uint16_t size);
        void FreeMemory(Memory mem);

        // Write and Read operations translate virtual address into physical
        void WriteRAM(int address, int value);
        uint16_t ReadRAM(int address);

        // Segment control operations
        Segment InitSegment(int direction);
        void WriteSegment(Segment segment, int address, int value);
        uint16_t ReadSegment(Segment segment, int address);

    private:
        void MoveToSwap(int pageNumber); // TODO: this function needs IOControl
        int FindLeastAccessedPage();
        std::vector<int> GetAddressList(std::vector<int> pages);
};