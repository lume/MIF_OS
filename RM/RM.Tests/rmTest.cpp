#include "rmTest.h"

// Set up Test Environment
RmTest::RmTest()
{}

// Tear down Test Environment
RmTest::~RmTest()
{}

void RmTest::RunAllTests()
{}

bool RmTest::LoadProgramTest_GivenRamIsEnough_AllocateMemoryCorrectly()
{
    
}

bool RmTest::LoadProgramTest_GivenNotEnoughRam_AllocateMemoryOnSwap(){}

bool RmTest::LoadProgramTest_GivenNotEnoughMemory_FailToAllocate(){}

bool RmTest::LoadProgramTest_GivenProgramTooBigForSinglePage_AllocateOnMultiplePages(){}

bool RmTest::ExecuteProgramTest_GivenLoadedProgram_ExecuteSuccesfully(){}

bool RmTest::ExecuteProgramTest_GivenLoadedProgramMultiPages_ExecuteSuccesfully(){}