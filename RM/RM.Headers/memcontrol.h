#pragma once
#include <array>
#include <map>
#include <vector>
#include "IOControl.h"
#include "SizeDefinitions.h"
#include <limits>

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
    int pc = 0x0;   // program counter
    int addr = 0x0; // internal addr register
    int acc = 0x0;   // accumulator
    int ir = 0x0;   // instruction register
    int sp = RAM_SIZE - 1; // stack pointer 
    int fs = 0x0; // flags
    int xReg = 0x0; // x register
    int cReg = 0x0; // c register
    int retReg = 0x0; // ret register
};


struct Program
{
    Segment dataSegment;
    Segment codeSegment;
    Segment stackSegment;
    CpuSnapshot cpuSnapshot;
};

struct Process
{
    int id;
    std::string name;
    Program program;
    int parent;
    std::vector<Process> childProcesses;
    int status; // 0 dead 1 alive 2 zombie
};

struct HeapBlockHandler
{
    Program owner;
    int size;
    int start;
    bool free;
};

inline std::array<int, RAM_SIZE> RAM = {0};
inline std::array<int, VRAM_SIZE> VRAM = {0};
inline std::array<Page, PAGETABLE_SIZE> pageTable;
inline std::array<int, FRAMETABLE_SIZE> frameTable;
inline std::vector<HeapBlockHandler> HeapBlockHandlers;
inline std::vector<Process> processList;

class Memcontrol
{
    public: 
        Memcontrol();
        int activeProcessId;

        Memory AllocateMemory(uint16_t size, std::vector<int> pagesToIgnore = {});
        void FreeMemory(Memory mem);

        // Write and Read operations translate virtual address into physical
        void WriteRAM(int address, int value);
        uint16_t ReadRAM(int address);

        // Segment control operations
        Segment InitSegment(int direction, int pageCount = 1);
        void WriteSegment(Segment segment, int address, int value);
        uint16_t ReadSegment(Segment segment, int address);

        int GetVarAddrIfExists(Program program, int var); //returns an address for a new variable
        int FindVarAddress(Program program, int var);
        int FindPtrAddress();

        Program PrepareProgramMemory(Program program);
        int ConvertToPhysAddress(int addr);
        
        int MoveToSwap(int pageNumber); 

        HeapBlockHandler HeapAlloc(Program owner, int size);
        void HeapFree(HeapBlockHandler *handler);
        void StoreStringInHeap(HeapBlockHandler handler, std::string str);
        std::string ReadStringFromHeap(HeapBlockHandler handler);

        int ForkProcess(std::string programName, Program program); //returns new process id
        void StopCurrentProcess(); // stops current process and sets it's parent process as active

    private:
        std::vector<int> freeFramePool;
        std::vector<int> freeSectorPool;

        IOControl iocontroller = IOControl();

        void ClearPageBeforeUse(int page);

        std::array<int, PAGE_SIZE> GetFromSwap(int pageNumber);
        int FindLeastAccessedPage(std::vector<int> collectedPages = {});
        std::vector<int> GetAddressList(std::vector<int> pages);
};