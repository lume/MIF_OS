#include "rmTest.h"
#include <vector>
#include <iostream>

// Set up Test Environment
RmTest::RmTest()
{}

// Tear down Test Environment
RmTest::~RmTest()
{}

void RmTest::RunAllTests()
{
    std::cout << "LoadProgramTest_GivenRamIsEnough_AllocateMemoryCorrectly...";
    if(LoadProgramTest_GivenRamIsEnough_AllocateMemoryCorrectly())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }

    std::cout << "LoadProgramTest_GivenNotEnoughRam_MoveMemoryToSwap...";
    if(LoadProgramTest_GivenNotEnoughRam_MoveMemoryToSwap())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }
    
    std::cout << "LoadProgramTest_GivenNotEnoughMemory_FailToAllocate(UNSTABLE)...";
    if(LoadProgramTest_GivenNotEnoughMemory_FailToAllocate())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }

    std::cout << "LoadProgramTest_GivenProgramTooBigForSinglePage_AllocateOnMultiplePages...";
    if(LoadProgramTest_GivenProgramTooBigForSinglePage_AllocateOnMultiplePages())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }
    
    std::cout << "ExecuteProgramTest_GivenLoadedProgram_ExecuteSuccesfully...";
    if(ExecuteProgramTest_GivenLoadedProgram_ExecuteSuccesfully())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }

    std::cout << "ExecuteProgramTest_GivenLoadedProgramMultiPages_ExecuteSuccesfully...";
    if(ExecuteProgramTest_GivenLoadedProgramMultiPages_ExecuteSuccesfully())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }

    std::cout << "ExecuteProgramTest_WhenMemoryOnSwap_RetrievesMemoryFromSwap...";
    if(ExecuteProgramTest_WhenMemoryOnSwap_RetrievesMemoryFromSwap())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }

    std::cout << "ExecuteProgramTest_AfterExecution_MemoryIsFreed...";
    if(ExecuteProgramTest_AfterExecution_MemoryIsFreed())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }

    std::cout << "ExecuteProgramTest_MemoryIsFreed_PagesCanBeAccessed...";
    if(ExecuteProgramTest_MemoryIsFreed_PagesCanBeAccessed())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }

    std::cout << "ExecuteProgramTest_SeveralProgramsExecuted_ExecuteSuccesfully...";
    if(ExecuteProgramTest_SeveralProgramsExecuted_ExecuteSuccesfully())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }

    std::cout << "LoadProgramTest_GivenSeveralPrograms_LoadedSuccesfully...";
    if(LoadProgramTest_GivenSeveralPrograms_LoadedSuccesfully())
    {
        std::cout << "PASSED" << std::endl;
    }
    else 
    {
        std::cout << "FAILED" << std::endl;
    }
    
}

bool RmTest::LoadProgramTest_GivenRamIsEnough_AllocateMemoryCorrectly()
{
    Cpu cpu = Cpu();
    Program prog = cpu.LoadProgram("small.txt");
    std::vector<int> expectedCodeSegm = {46, 97, 2, 10, 51, 97, 46, 98, 2, 100, 51, 98, 48, 97, 5, 120, 48, 98, 8, 120, 46, 99, 51, 99};

    if(prog.dataSegment.startPointer == prog.codeSegment.startPointer)
        return false;
    if(prog.codeSegment.startPointer == prog.stackSegment.startPointer)
        return false;
    if(prog.dataSegment.startPointer == prog.stackSegment.startPointer)
        return false;
    
    for(int i = 0; i < expectedCodeSegm.size(); i++)
    {
        if(RAM[prog.codeSegment.memory.addresses[i]] != expectedCodeSegm[i])
            return false;
    }

    return true;
}

bool RmTest::LoadProgramTest_GivenNotEnoughRam_MoveMemoryToSwap()
{
    Cpu cpu = Cpu();
    Memcontrol memc = Memcontrol();
    std::vector<int> expectedCodeSegm = {46, 97, 2, 10, 51, 97, 46, 98, 2, 100, 51, 98, 48, 97, 5, 120, 48, 98, 8, 120, 46, 99, 51, 99, 0};

    for(int i = 0; i < 64; i++)
    {
        cpu.LoadProgram("big.txt");
    }

    Program prog = cpu.LoadProgram("small.txt");

    for(int i = 0; i < expectedCodeSegm.size(); i++)
    {
        int a1 = RAM[memc.ConvertToPhysAddress(prog.codeSegment.memory.addresses[i])];
        int a2 = expectedCodeSegm[i];
        if(a1 != a2)
            return false;
    }
    
    return true;
}

bool RmTest::LoadProgramTest_GivenNotEnoughMemory_FailToAllocate()
{
    Cpu cpu = Cpu();

    try
    {
        for(int i = 0; i < 200; i++)
        {
            Program p = cpu.LoadProgram("big.txt");
        }

    
        cpu.LoadProgram("small.txt");
    }
    catch(std::exception *error)
    {
        return true;
    }

    return false;
}

bool RmTest::LoadProgramTest_GivenProgramTooBigForSinglePage_AllocateOnMultiplePages()
{
    Cpu cpu = Cpu();
    Program program = cpu.LoadProgram("big.txt");

    return program.codeSegment.memory.usedPages.size() == 2;
}

bool RmTest::ExecuteProgramTest_GivenLoadedProgram_ExecuteSuccesfully()
{
    Cpu cpu = Cpu();
    Program program = cpu.LoadProgram("small.txt");
    std::vector<int> expectedDataSegm = {97, 10, 98, 100, 99, 110};

    cpu.ExecuteProgram(program);

    for(int i = 0; i < expectedDataSegm.size(); i++)
    {
        int x = RAM[program.dataSegment.memory.addresses[i]];
        if(x != expectedDataSegm[i])
            return false;
    }

    return true;
}

bool RmTest::ExecuteProgramTest_GivenLoadedProgramMultiPages_ExecuteSuccesfully()
{
    Cpu cpu = Cpu();
    Program program = cpu.LoadProgram("big.txt");
    std::vector<int> expectedDataSegm = {122, 4800};

    cpu.ExecuteProgram(program, 10000);

    for(int i = 0; i < expectedDataSegm.size(); i++)
    {
        if(RAM[program.dataSegment.memory.addresses[i]] != expectedDataSegm[i])
            return false;
    }

    return true;
}

bool RmTest::ExecuteProgramTest_WhenMemoryOnSwap_RetrievesMemoryFromSwap()
{
    Cpu cpu = Cpu();
    Program programToBeExecuted= cpu.LoadProgram("small.txt");
    std::vector<int> expectedDataSegm = {97, 10, 98, 100, 99, 110};


    cpu.memcontroller.MoveToSwap(programToBeExecuted.codeSegment.memory.usedPages[0]);
    cpu.memcontroller.MoveToSwap(programToBeExecuted.dataSegment.memory.usedPages[0]);

    cpu.ExecuteProgram(programToBeExecuted);

    for(int i = 0; i < expectedDataSegm.size(); i++)
    {
        if(RAM[programToBeExecuted.dataSegment.memory.addresses[i]] != expectedDataSegm[i])
            return false;
    }

    return true;
}

bool RmTest::ExecuteProgramTest_AfterExecution_MemoryIsFreed()
{
    Cpu cpu = Cpu();
    std::vector<int> usedPages;

    Program program = cpu.LoadProgram("small.txt");
    usedPages.insert(usedPages.end(), program.codeSegment.memory.usedPages.begin(),
    program.codeSegment.memory.usedPages.end());

    usedPages.insert(usedPages.end(), program.stackSegment.memory.usedPages.begin(),
    program.stackSegment.memory.usedPages.end());

    usedPages.insert(usedPages.end(), program.dataSegment.memory.usedPages.begin(),
    program.dataSegment.memory.usedPages.end());

    cpu.ExecuteProgram(program);

    for(int i : usedPages)
    {
        if(pageTable[i].used)
            return false;
    }

    return true;
}

bool RmTest::ExecuteProgramTest_MemoryIsFreed_PagesCanBeAccessed()
{
    Cpu cpu = Cpu();

    try
    {
        for(int i = 0; i < 200; i++)
        {
            Program p = cpu.LoadProgram("big.txt");
            cpu.ExecuteProgram(p);
        }

        Program newProgram = cpu.LoadProgram("small.txt");
        cpu.ExecuteProgram(newProgram);

        std::vector<int> expectedDataSegm = {97, 10, 98, 100, 99, 110};

        for(int i = 0; i < expectedDataSegm.size(); i++)
        {
            int x = RAM[newProgram.dataSegment.memory.addresses[i]];
            if(x != expectedDataSegm[i])
                return false;
        }
    }
    catch(std::exception *error)
    {
        return false;
    }

    return true;
}

bool RmTest::ExecuteProgramTest_SeveralProgramsExecuted_ExecuteSuccesfully()
{
    Cpu cpu = Cpu();
    Program p1 = cpu.LoadProgram("small.txt");
    Program p2 = cpu.LoadProgram("big.txt");

    std::vector<int> expectedDataSegm1 = {97, 10, 98, 100, 99, 110};
    std::vector<int> expectedDataSegm2 = {122, 4800};

    cpu.ExecuteProgram(p1);
    cpu.ExecuteProgram(p2);

    for(int i = 0; i < expectedDataSegm1.size(); i++)
    {
        int x = RAM[p1.dataSegment.memory.addresses[i]];
        if(x != expectedDataSegm1[i])
            return false;
    }

    for(int i = 0; i < expectedDataSegm2.size(); i++)
    {
        int x = RAM[p2.dataSegment.memory.addresses[i]];
        if(x != expectedDataSegm2[i])
            return false;
    }

    return true;
}

bool RmTest::LoadProgramTest_GivenSeveralPrograms_LoadedSuccesfully()
{
    Cpu cpu = Cpu();
    Program p1 = cpu.LoadProgram("small.txt");
    Program p2 = cpu.LoadProgram("small1.txt");

    std::vector<int> expectedCodeSegm = {46, 97, 2, 10, 51, 97, 46, 98, 2, 100, 51, 98, 48, 97, 5, 120, 48, 98, 8, 120, 46, 99, 51, 99, 0};
    std::vector<int> expectedCodeSegm1 = {2, 12, 46, 120, 51, 120, 2, 10, 26, 0};

    if(p1.dataSegment.startPointer == p2.dataSegment.startPointer)
        return false;
    if(p1.codeSegment.startPointer == p2.codeSegment.startPointer)
        return false;
    if(p1.stackSegment.startPointer == p2.stackSegment.startPointer)
        return false;
    
    for(int i = 0; i < expectedCodeSegm.size(); i++)
    {
        if(RAM[p1.codeSegment.memory.addresses[i]] != expectedCodeSegm[i])
            return false;
    }

    for(int i = 0; i < expectedCodeSegm1.size(); i++)
    {
        if(RAM[p2.codeSegment.memory.addresses[i]] != expectedCodeSegm1[i])
            return false;
    }

    return true;
}
