#include "memcontrol.h"

Memory Memcontrol::AllocateMemory(uint16_t size)
{
    std::vector<int> memPageNumbers;
    int pageCount = 0;

    if(size % 4 == 0)
        pageCount = size/4096;    
    else
        pageCount = size/4096 + 1;

    for(int i = 0; i < PAGETABLE_SIZE; i++)
    {
        if(memPageNumbers.size() == pageCount)
            continue;
        else if(!pageTable[i].used && pageTable[i].frame < 256)
        {
            pageTable[i].used = true;
            memPageNumbers.push_back(i);
        }
    }

    for(int i = 0; i < pageCount - memPageNumbers.size(); i++)
    {
        int leastAccessed = FindLeastAccessedPage();
        MoveToSwap(leastAccessed);
        memPageNumbers.push_back(leastAccessed);
        pageTable[leastAccessed].used = true;
    }

    if(memPageNumbers.size() != pageCount)
        throw new std::runtime_error("Out of memory :("); // If we can't find memory to swap too, it means we are out of memory
    else
        return {memPageNumbers};
}

void Memcontrol::FreeMemory(Memory mem)
{
    for(auto &page : mem.usedPages)
    {
        pageTable[page].used = false;
    }
}

int Memcontrol::FindLeastAccessedPage()
{
    int leastAccessed = 999;
    for(int i = 0; i < PAGETABLE_SIZE; i++)
    {
        if(leastAccessed > pageTable[i].timesAccessed && pageTable[i].used == false)
            leastAccessed = pageTable[i].timesAccessed;
    }
    return leastAccessed;
}

void Memcontrol::WriteRAM(int address, int value)
{
    //TODO: Add memory safety (check if address is in bounds of the page)
    int pageNumber = address & 0b11111111100000000000;
    int offset = address &0b00000000011111111111;
    int frameNumber = pageTable[pageNumber].frame;
    int physAddress = frameNumber + offset;

    RAM[physAddress] = value;
}

uint16_t Memcontrol::ReadRAM(int address)
{
    //TODO: Add memory safety (check if address is in bounds of the page)
    int pageNumber = address & 0b11111111100000000000;
    int offset = address &0b00000000011111111111;
    int frameNumber = pageTable[pageNumber].frame;
    int physAddress = frameNumber + offset;

    return RAM[physAddress];
}

void Memcontrol::MoveToSwap(int pageNumber)
{}

void Memcontrol::WriteSegment(Segment segment, int address, int value)
{

}

uint16_t Memcontrol::ReadSegment(Segment segment, int address){}