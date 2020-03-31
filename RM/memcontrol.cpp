#include "memcontrol.h"
#include <algorithm>
#include <iostream>

Memory Memcontrol::AllocateMemory(uint16_t size, std::vector<int> pagesToIgnore)
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
            memPageNumbers.push_back(i);
            pageTable[i].used = true;
        }
    }

    int collectedPageListSize = memPageNumbers.size();
    
    for(int i = 0; i < pageCount - collectedPageListSize; i++)
    {
        int leastAccessed = FindLeastAccessedPage(pagesToIgnore);
        if(!MoveToSwap(leastAccessed))
            throw new std::runtime_error("Out of memory :("); // If we can't find memory to swap too, it means we are out of memory
        
        memPageNumbers.push_back(leastAccessed);
        pageTable[leastAccessed].used = true;
    }

    return {memPageNumbers, GetAddressList(memPageNumbers)};
}

void Memcontrol::FreeMemory(Memory mem)
{
    for(auto &page : mem.usedPages)
    {
        pageTable[page].used = false;
    }
}

int Memcontrol::FindLeastAccessedPage(std::vector<int> collectedPages)
{
    int leastAccessed = 999;
    int page;
    for(int i = 0; i < FRAMETABLE_SIZE; i++)
    {
        if(leastAccessed > pageTable[i].timesAccessed &&
        (std::find(collectedPages.begin(), collectedPages.end(), i) == collectedPages.end()))
        {
            leastAccessed = pageTable[i].timesAccessed;
            page = i;
        }
    }
    if(leastAccessed > PAGETABLE_SIZE + FRAMETABLE_SIZE)
        throw new std::runtime_error("Out of memory :(");
    else
        return page;
}

void Memcontrol::WriteRAM(int address, int value)
{
    //TODO: Add memory safety (check if address is in bounds of the page)
    int pageNumber = address & 0b111111111100000000000;
    pageNumber = pageNumber >> 12;
    int offset = address - PAGE_SIZE*pageNumber;
    int frameNumber = pageTable[pageNumber].frame;
    int physAddress = frameNumber * PAGE_SIZE + offset;

    pageTable[pageNumber].timesAccessed++;
    pageTable[pageNumber].used = true;

    RAM[physAddress] = value;
}

uint16_t Memcontrol::ReadRAM(int address)
{
    //TODO: Add memory safety (check if address is in bounds of the page)
    int pageNumber = address & 0b111111111100000000000;
    pageNumber = pageNumber >> 12;
    int offset = address - PAGE_SIZE*pageNumber;
    int frameNumber = pageTable[pageNumber].frame;
    int physAddress = frameNumber * PAGE_SIZE + offset;

    pageTable[pageNumber].used = true;
    pageTable[pageNumber].timesAccessed++;

    return RAM[physAddress];
}

bool Memcontrol::MoveToSwap(int pageNumber)
{
    std::array<int, PAGE_SIZE> pageData;
    int memStart = pageTable[pageNumber].frame * PAGE_SIZE;

    for(int i = 0; i < PAGE_SIZE; i++)
    {
        pageData[i] = RAM[memStart+i];
    }

    int foundSector = -1;
    for(int i = FRAMETABLE_SIZE; i < PAGETABLE_SIZE; i++)
    {
        if(!pageTable[i].used)
        {
            foundSector = pageTable[i].swapSector;
            pageTable[i].used = true;
            break;
        }
    }

    if(foundSector == -1)
    {
        return false;
    }
    else
    {
        pageTable[pageNumber].swapSector = foundSector;
        iocontroller.WriteSwapData(foundSector, pageData);
        return true;
    }
}

void Memcontrol::GetFromSwap(int pageNumber)
{
    std::array<int, PAGE_SIZE> data = iocontroller.ReadSwapData(pageTable[pageNumber].swapSector);
    for(int i = 0; i < PAGE_SIZE; i++)
    {
        RAM[pageTable[pageNumber].frame+i] = data[i];
    }
    pageTable[pageNumber].used = false;
    pageTable[pageNumber].swapSector = -1;
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

Segment Memcontrol::InitSegment(int direction, Program program, int pageCount)
{
    Segment segment;
    int pageNumber;
    
    std::vector<int> pagesToIgnore;

    pagesToIgnore.insert(pagesToIgnore.end(), program.dataSegment.memory.usedPages.begin(),
    program.dataSegment.memory.usedPages.end());
    pagesToIgnore.insert(pagesToIgnore.end(), program.codeSegment.memory.usedPages.begin(),
    program.codeSegment.memory.usedPages.end());
    pagesToIgnore.insert(pagesToIgnore.end(), program.stackSegment.memory.usedPages.begin(),
    program.stackSegment.memory.usedPages.end());

    segment.direction = direction;

    segment.memory = AllocateMemory(PAGE_SIZE * pageCount, pagesToIgnore);

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
        pageTable[i].onDisk = false;
        pageTable[i].swapSector = -1;
        if(i >= FRAMETABLE_SIZE)
        {
            pageTable[i].onDisk = true;
            pageTable[i].swapSector = i - FRAMETABLE_SIZE;
        }
    }
}

int Memcontrol::FindPtrAddress()
{
    return 0;
}

int Memcontrol::FindVarAddress(Program program, int var)
{
    int startAddress = program.dataSegment.startPointer;
    for(int i = 0; i < program.dataSegment.memory.addresses.size(); i+=2)
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
    for(int i = 0; i < program.dataSegment.memory.addresses.size(); i+=2)
    {
        if(RAM[startAddress+i] == var)
        {
            return true;
        }
    }
    return false;
}  