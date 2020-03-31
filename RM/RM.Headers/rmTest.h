#pragma once 

#include "Clock.h"
#include "cpu.h"
#include "memcontrol.h"
#include "IOControl.h"

class RmTest
{
    public:
        RmTest();
        ~RmTest();

        void RunAllTests();

    private:
        bool LoadProgramTest_GivenRamIsEnough_AllocateMemoryCorrectly();
        bool LoadProgramTest_GivenNotEnoughRam_AllocateMemoryOnSwap();
        bool LoadProgramTest_GivenNotEnoughMemory_FailToAllocate();
        bool LoadProgramTest_GivenProgramTooBigForSinglePage_AllocateOnMultiplePages();
        bool ExecuteProgramTest_GivenLoadedProgram_ExecuteSuccesfully();
        bool ExecuteProgramTest_GivenLoadedProgramMultiPages_ExecuteSuccesfully();
};