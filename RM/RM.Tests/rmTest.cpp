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
    
    std::cout << "LoadProgramTest_GivenNotEnoughMemory_FailToAllocate...";
    if(LoadProgramTest_GivenNotEnoughMemory_FailToAllocate())
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
    std::vector<int> expectedCodeSegm = {46, 97, 2, 10, 51, 97, 46, 98, 2, 100, 51, 98, 48, 97, 5, 120, 48, 98, 8, 120, 46, 99, 51, 99};

    for(int i = 0; i < 64; i++)
    {
        cpu.LoadProgram("big.txt");
    }

    Program prog = cpu.LoadProgram("small.txt");

    for(int i = 0; i < expectedCodeSegm.size(); i++)
    {
        if(RAM[prog.codeSegment.memory.addresses[i]] != expectedCodeSegm[i])
            return false;
    }
    
    return true;
}

bool RmTest::LoadProgramTest_GivenNotEnoughMemory_FailToAllocate()
{
    Cpu cpu = Cpu();

    for(int i = 0; i < 128; i++)
    {
        cpu.LoadProgram("big.txt");
    }

    try
    {
        cpu.LoadProgram("small.txt");
    }
    catch(std::exception *error)
    {
        return true;
    }

    return false;
}

bool RmTest::LoadProgramTest_GivenProgramTooBigForSinglePage_AllocateOnMultiplePages(){}

bool RmTest::ExecuteProgramTest_GivenLoadedProgram_ExecuteSuccesfully(){}

bool RmTest::ExecuteProgramTest_GivenLoadedProgramMultiPages_ExecuteSuccesfully(){}