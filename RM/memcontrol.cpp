#include "memcontrol.h"
#include <algorithm>
#include <iostream>

Memory Memcontrol::AllocateMemory(uint16_t size)
{
    std::vector<int> memPageNumbers;
    int pageCount = 0;

    if(size % PAGE_SIZE == 0)
        pageCount = size/PAGE_SIZE;    
    else
        pageCount = size/PAGE_SIZE + 1;



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
        return {memPageNumbers, GetAddressList(memPageNumbers)};
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
    if(leastAccessed > PAGETABLE_SIZE + FRAMETABLE_SIZE)
        throw new std::runtime_error("Out of memory :(");
    else
        return leastAccessed;
}

void Memcontrol::WriteRAM(int address, int value)
{
    //TODO: Add memory safety (check if address is in bounds of the page)
    int pageNumber = address & 0b111111111100000000000;
    pageNumber = pageNumber >> 12;
    int offset = address &0b000000000011111111111;
    int frameNumber = pageTable[pageNumber].frame;
    int physAddress = frameNumber * 4096 + offset;

    pageTable[pageNumber].timesAccessed++;

    RAM[physAddress] = value;
}

uint16_t Memcontrol::ReadRAM(int address)
{
    //TODO: Add memory safety (check if address is in bounds of the page)
    int pageNumber = address & 0b111111111100000000000;
    pageNumber = pageNumber >> 12;
    int offset = address & 0b000000000011111111111;
    int frameNumber = pageTable[pageNumber].frame;
    int physAddress = frameNumber * 4096 + offset;

    pageTable[pageNumber].timesAccessed++;

    return RAM[physAddress];
}

void Memcontrol::MoveToSwap(int pageNumber)
{
    std::array<int, PAGE_SIZE> pageData;
    int memStart = pageTable[pageNumber].frame * PAGE_SIZE;

    for(int i = 0; i < PAGE_SIZE; i++)
    {
        pageData[i] = memStart+i;
    }

    iocontroller.WriteSwapData(pageNumber, pageData);
}

void Memcontrol::GetFromSwap(int pageNumber)
{
    std::array<int, PAGE_SIZE> data = iocontroller.ReadSwapData(pageNumber+256);
    for(int i = 0; i < PAGE_SIZE; i++)
    {
        RAM[pageTable[pageNumber].frame+i] = data[i];
    }
}

void Memcontrol::WriteSegment(Segment segment, int address, int value)
{
    if(std::find(segment.memory.addresses.begin(), segment.memory.addresses.end(), address) != segment.memory.addresses.end())
    {
        WriteRAM(address, value);
    }
    else{
        throw std::runtime_error("Trying to write into forbidden memory");
    }
}

uint16_t Memcontrol::ReadSegment(Segment segment, int address){
    //TODO: implement
    return 0;
}

Segment Memcontrol::InitSegment(int direction)
{
    Segment segment;
    int pageNumber;
    
    segment.direction = direction;
    segment.memory = AllocateMemory(1<<12);
    pageNumber = segment.memory.usedPages[0];
    segment.writePointer = (pageNumber << 12) & 0b11111111100000000000;
    segment.startPointer = (pageNumber << 12) & 0b11111111100000000000;
    return segment;
}

std::vector<int> Memcontrol::GetAddressList(std::vector<int> pages)
{
    std::vector<int> addressList;
    int z = 0b111111111111;

    for(auto &p : pages)
    {
        int k = p << 12;
        for(int i = 0; i < z; i++)
        {
            addressList.push_back(k + i);
        }
    }

    int x = addressList.size();

    return addressList;
}

Memcontrol::Memcontrol()
{
    for(int i = 0; i < PAGETABLE_SIZE; i++)
    {
        pageTable[i].frame = i;
        pageTable[i].timesAccessed = 0;
        pageTable[i].used = false;
    }
}

int Memcontrol::FindPtrAddress()
{
    return 0;
}

int Memcontrol::FindVarAddress(Program program, int var)
{
    int startAddress = program.dataSegment.startPointer;
    for(int i = 0; i < 4096; i+=2)
    {
        int p = RAM[startAddress+i];
        if(p == var)
        {
            return startAddress+i;
        }
    }

    throw new std::runtime_error("Failed to find variable");
}

bool Memcontrol::CheckIfVarExists(Program program, int var)
{
    int startAddress = program.dataSegment.startPointer;
    for(int i = 0; i < 4096; i+=2)
    {
        if(RAM[startAddress+i] == var)
        {
            return true;
        }
    }
    return false;
}  