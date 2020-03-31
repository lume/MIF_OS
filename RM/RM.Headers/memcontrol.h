#pragma once
#include <array>
#include <map>
#include <vector>
#include "IOControl.h"
#include "SizeDefinitions.h"

//pages and frames are fixed size (4kb)

struct Page 
{
    bool used;
    bool onDisk;
    int timesAccessed;
    int frame; 
    int swapSector;
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
    int writePointer;  // Pointer to the piece of segment where last write occured
    int startPointer; // Pointer to the start of segment
    char direction;   // if is set to 1, memory write/read is backward (stack)
};

struct CpuSnapshot
{
    uint16_t pc = 0x0;   // program counter
    uint16_t addr = 0x0; // internal addr register
    uint16_t acc = 0x0;   // accumulator
    uint16_t ir = 0x0;   // instruction register
    int sp = RAM_SIZE - 1; // stack pointer 
    uint16_t fs = 0x0; // flags
    uint16_t xReg = 0x0; // x register
    uint16_t cReg = 0x0; // c register
};


struct Program
{
    Segment dataSegment;
    Segment codeSegment;
    Segment stackSegment;
    CpuSnapshot cpuSnapshot;
};

inline std::array<uint16_t, RAM_SIZE> RAM = {0};
inline std::array<uint16_t, VRAM_SIZE> VRAM = {0};
inline std::array<Page, PAGETABLE_SIZE> pageTable;
inline std::array<int, FRAMETABLE_SIZE> frameTable;

class Memcontrol
{
    public: 
        Memcontrol();

        Memory AllocateMemory(uint16_t size, std::vector<int> pagesToIgnore = {});
        void FreeMemory(Memory mem);

        // Write and Read operations translate virtual address into physical
        void WriteRAM(int address, int value);
        uint16_t ReadRAM(int address);

        // Segment control operations
        Segment InitSegment(int direction, Program program, int pageCount = 1);
        void WriteSegment(Segment segment, int address, int value);
        uint16_t ReadSegment(Segment segment, int address);

        bool CheckIfVarExists(Program program, int var); //returns an address for a new variable
        int FindVarAddress(Program program, int var);
        int FindPtrAddress();

    private:
        IOControl iocontroller = IOControl();

        bool MoveToSwap(int pageNumber); 
        void GetFromSwap(int pageNumber);
        int FindLeastAccessedPage(std::vector<int> collectedPages);
        std::vector<int> GetAddressList(std::vector<int> pages);
};