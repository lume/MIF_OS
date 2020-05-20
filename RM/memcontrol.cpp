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
        else if(!pageTable[i].used && !pageTable[i].onDisk && pageTable[i].frame != -1) 
        {
            memPageNumbers.push_back(i);
            pageTable[i].used = true;
        }
    }

    int collectedPageListSize = memPageNumbers.size();
    
    for(int i = 0; i < pageCount - collectedPageListSize; i++)
    {
        int leastAccessed = 0;
        int ptSize = PAGETABLE_SIZE;
        leastAccessed = FindLeastAccessedPage(pagesToIgnore);

        if(leastAccessed > ptSize || leastAccessed == -1)
        {
            throw new std::runtime_error("Out of memory :(");
        }
        
        int newPage = MoveToSwap(leastAccessed);
        if(newPage == -1 || pageTable[newPage].frame == -1)
            throw new std::runtime_error("Out of memory :("); // If we can't find memory to swap too, it means we are out of memory
        
        memPageNumbers.push_back(newPage);
        pageTable[newPage].used = true;
    }

    for(int i : memPageNumbers)
    {
        ClearPageBeforeUse(i);
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
    int leastAccessed = std::numeric_limits<int>::max();
    int page = -1;
    for(int i = 0; i < PAGETABLE_SIZE; i++)
    {
        if(leastAccessed > pageTable[i].timesAccessed && !pageTable[i].onDisk && 
        pageTable[i].frame != -1 &&
        (std::find(collectedPages.begin(), collectedPages.end(), i) == collectedPages.end()))
        {
            leastAccessed = pageTable[i].timesAccessed;
            if(pageTable[i].frame == -1)
                throw;
            page = i;
        }
    }

 
    return page;
}

void Memcontrol::WriteRAM(int address, int value)
{
    //TODO: Add memory safety (check if address is in bounds of the page)
    int physAddress = ConvertToPhysAddress(address);

    RAM[physAddress] = value;
}

uint16_t Memcontrol::ReadRAM(int address)
{
    //TODO: Add memory safety (check if address is in bounds of the page)
    int physAddress = ConvertToPhysAddress(address);

    return RAM[physAddress];
}

int Memcontrol::MoveToSwap(int pageNumber)
{
    std::array<int, PAGE_SIZE> pageData;
    int memStart = pageTable[pageNumber].frame * PAGE_SIZE;
   
    for(int i = 0; i < PAGE_SIZE; i++)
    {
        if((memStart+i) > RAM.size())
        {    
            throw new std::runtime_error("Invalid memory access!");
        }
        pageData[i] = RAM[memStart+i];
    }

    int foundSector = -1;
    int foundNewPage = -1;
    for(int i = 0; i < PAGETABLE_SIZE; i++)
    {
        if(!pageTable[i].used && !pageTable[i].onDisk && i != pageNumber && pageTable[i].swapSector != -1)
        {
            foundSector = pageTable[i].swapSector;
            foundNewPage = i;
            break;
        }
    }

    if(foundSector == -1)
    {
        return -1;
    }
    else
    {
        pageTable[pageNumber].swapSector = foundSector;
        iocontroller.WriteSwapData(foundSector, pageData);

        pageTable[foundNewPage].onDisk = false;
        pageTable[foundNewPage].swapSector = -1;
        pageTable[foundNewPage].used = false;
        pageTable[foundNewPage].frame = pageTable[pageNumber].frame;

        pageTable[pageNumber].used = false;
        pageTable[pageNumber].frame = -1;
        pageTable[pageNumber].onDisk = true;
        return foundNewPage;
    }
}

std::array<int, PAGE_SIZE> Memcontrol::GetFromSwap(int pageNumber)
{
    std::array<int, PAGE_SIZE> data = iocontroller.ReadSwapData(pageTable[pageNumber].swapSector);
    return data;
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

Segment Memcontrol::InitSegment(int direction, int pageCount)
{
    Segment segment;
    int pageNumber;
    
    std::vector<int> pagesToIgnore;

    segment.direction = direction;

    segment.memory = AllocateMemory(PAGE_SIZE * pageCount);

    pageNumber = segment.memory.usedPages[0];
    //segment.writePointer = (pageNumber << 12) & 0b11111111100000000000;
    //segment.startPointer = (pageNumber << 12) & 0b11111111100000000000;
    segment.writePointer = pageTable[pageNumber].frame * PAGE_SIZE;
    segment.startPointer = pageTable[pageNumber].frame * PAGE_SIZE;

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
    activeProcessId = -1;
    for(int i = 0; i < PAGETABLE_SIZE; i++)
    {
        pageTable[i].frame = i;
        pageTable[i].timesAccessed = 0;
        pageTable[i].used = false;
        pageTable[i].onDisk = false;
        pageTable[i].swapSector = -1;
        if(i >= FRAMETABLE_SIZE)
        {
            pageTable[i].swapSector = i - FRAMETABLE_SIZE;
            pageTable[i].frame = -1;
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

int Memcontrol::GetVarAddrIfExists(Program program, int var)
{
    int startAddress = ConvertToPhysAddress(program.dataSegment.startPointer);
    int varAddr = -1;
    for(int i = 0; i < program.dataSegment.memory.addresses.size(); i+=2)
    {
        if(RAM[startAddress+i] == var)
        {
            varAddr = startAddress+i;
        }
    }
    return varAddr;
}  

Program Memcontrol::PrepareProgramMemory(Program program)
{
    std::vector<int> pagesToIgnore;
    pagesToIgnore.insert(pagesToIgnore.end(),
    program.codeSegment.memory.usedPages.begin(),
    program.codeSegment.memory.usedPages.end());
    
    pagesToIgnore.insert(pagesToIgnore.end(),
    program.stackSegment.memory.usedPages.begin(),
    program.stackSegment.memory.usedPages.end());

    pagesToIgnore.insert(pagesToIgnore.end(),
    program.dataSegment.memory.usedPages.begin(),
    program.dataSegment.memory.usedPages.end());

    for(auto i : program.codeSegment.memory.usedPages)
    {
        if(pageTable[i].onDisk || pageTable[i].frame == -1)
        {
            int lu = FindLeastAccessedPage(pagesToIgnore);
            int newPage = MoveToSwap(lu);
            std::array<int, PAGE_SIZE> data = GetFromSwap(i);
            pageTable[newPage].swapSector = pageTable[i].swapSector;
            pageTable[newPage].onDisk = true;
            pageTable[i].onDisk = false;
            pageTable[i].used = true;
            pageTable[newPage].used = false;
            pageTable[i].frame = pageTable[newPage].frame;
            pageTable[i].swapSector = -1;
            pageTable[newPage].frame = -1;
            for(int j = 0; j < PAGE_SIZE; j++)
            {
                int addr = pageTable[i].frame * PAGE_SIZE;
                RAM[addr + j] = data[j];
            }
        }
    }

    for(auto i : program.stackSegment.memory.usedPages)
    {
        if(pageTable[i].onDisk || pageTable[i].frame == -1)
        {
            int lu = FindLeastAccessedPage(pagesToIgnore);
            int newPage = MoveToSwap(lu);
            std::array<int, PAGE_SIZE> data = GetFromSwap(i);
            pageTable[newPage].swapSector = pageTable[i].swapSector;
            pageTable[newPage].onDisk = true;
            pageTable[i].onDisk = false;
            pageTable[i].used = true;
            pageTable[newPage].used = false;
            pageTable[i].frame = pageTable[newPage].frame;
            pageTable[i].swapSector = -1;
            pageTable[newPage].frame = -1;
            for(int j = 0; j < PAGE_SIZE; j++)
            {
                int addr = pageTable[i].frame * PAGE_SIZE;
                RAM[addr + j] = data[j];
            }
        }
    }

    for(auto i : program.dataSegment.memory.usedPages)
    {
        if(pageTable[i].onDisk || pageTable[i].frame == -1)
        {
            int lu = FindLeastAccessedPage(pagesToIgnore);
            int newPage = MoveToSwap(lu);
            std::array<int, PAGE_SIZE> data = GetFromSwap(i);
            pageTable[newPage].swapSector = pageTable[i].swapSector;
            pageTable[newPage].onDisk = true;
            pageTable[i].onDisk = false;
            pageTable[i].used = true;
            pageTable[newPage].used = false;
            pageTable[i].frame = pageTable[newPage].frame;
            pageTable[i].swapSector = -1;
            pageTable[newPage].frame = -1;
            for(int j = 0; j < PAGE_SIZE; j++)
            {
                int addr = pageTable[i].frame * PAGE_SIZE;
                RAM[addr + j] = data[j];
            }
        }
    }

    program.codeSegment.memory.addresses = GetAddressList(program.codeSegment.memory.usedPages);
    program.stackSegment.memory.addresses = GetAddressList(program.stackSegment.memory.usedPages);
    program.dataSegment.memory.addresses = GetAddressList(program.dataSegment.memory.usedPages);

    /*program.codeSegment.startPointer = program.codeSegment.memory.addresses[0];
    program.stackSegment.startPointer = program.stackSegment.memory.addresses[program.stackSegment.memory.addresses.size()-1];
    program.dataSegment.startPointer = program.dataSegment.memory.addresses[0];
   
    program.codeSegment.writePointer = program.codeSegment.memory.addresses[0];
    program.stackSegment.writePointer = program.stackSegment.memory.addresses[program.stackSegment.memory.addresses.size()-1];
    program.dataSegment.writePointer = program.dataSegment.memory.addresses[0];*/

    return program;
}


int Memcontrol::ConvertToPhysAddress(int addr)
{
    int pageNumber = addr & 0b111111111100000000000;
    pageNumber = pageNumber >> 12;
    int offset = addr - PAGE_SIZE*pageNumber;
    int frameNumber = pageTable[pageNumber].frame;

    if(pageTable[pageNumber].onDisk)
    {
        throw new std::runtime_error("Unhandled page fault");
    }

    int physAddress = frameNumber * PAGE_SIZE + offset;

    pageTable[pageNumber].timesAccessed++;
    pageTable[pageNumber].used = true;
    
    return physAddress;
}

void Memcontrol::ClearPageBeforeUse(int page)
{
    std::vector<int> temp;
    temp.push_back(page);
    std::vector<int> addresses;

    addresses = GetAddressList(temp);

    int startAddr = pageTable[page].frame;
    for(int i = 0; i < addresses.size(); i++)
    {
        RAM[ConvertToPhysAddress(addresses[i])] = 0;
    }
}

HeapBlockHandler Memcontrol::HeapAlloc(Program owner, int size)
{
    HeapBlockHandler newBlock;

    for(auto block : HeapBlockHandlers)
    {
        if(block.free && block.size <= size)
        {
            //allocate here
            newBlock.start = block.start;
            newBlock.size = size;
            newBlock.owner = owner;
        }
    }
    if(HeapBlockHandlers.size() > 0 && 
    HeapBlockHandlers.back().start + HeapBlockHandlers.back().size + size <= RAM_SIZE)
    {
        //allocate here
        newBlock.start = HeapBlockHandlers.back().start + HeapBlockHandlers.back().size + 1;
        newBlock.size = size;
        newBlock.owner = owner;
    }
    else if(HeapBlockHandlers.size() == 0)
    {
        //allocate here
        newBlock.start = HEAP_START;
        newBlock.size = size;
        newBlock.owner = owner;
    }
    else
    {
        std::cout << "No more heap memory" << std::endl;
        throw std::runtime_error("No more heap memory");
    }
    newBlock.free = false;
    HeapBlockHandlers.insert(HeapBlockHandlers.end(), newBlock);
    return newBlock;
}

void Memcontrol::HeapFree(HeapBlockHandler *handler)
{
    handler->free = true;
}

void Memcontrol::StoreStringInHeap(HeapBlockHandler handler, std::string str)
{
    int memstart = handler.start;
    for(int i = 0; i < str.length(); i++)
    {
        RAM[memstart+i] = str[i];
    }
    RAM[memstart+str.length()+1] = 0;
}

std::string Memcontrol::ReadStringFromHeap(HeapBlockHandler handler)
{
    std::vector<int> contents;
    
    for(int i = 0; i < handler.size; i++)
    {
        int c = RAM[handler.start+i];
       
        if(c != 0)
            contents.insert(contents.end(), RAM[handler.start+i]);
    }
    contents.insert(contents.end(), 0);

    std::string str(contents.begin(), contents.end());
    return str;
}

int Memcontrol::ForkProcess(std::string programName, Program program)
{
    Process process;
    if(activeProcessId != -1)
        process.parent = processList[activeProcessId].id;
    process.name = programName;
    process.program = program;
    process.status = 1;
    process.id = processList.size();
    if(activeProcessId != -1)
        processList[activeProcessId].childProcesses.insert(processList[activeProcessId].childProcesses.end(), process);
    if(process.id == -1)
    {
        process.parent = -1;
        process.id = 0;
    }
    processList.insert(processList.end(), process);
    
    return process.id;
}

void Memcontrol::StopCurrentProcess()
{
    processList[activeProcessId].status = 0;
    for(auto p : processList[activeProcessId].childProcesses)
    {
        p.status = 2;
    }
    activeProcessId = processList[activeProcessId].parent;
}