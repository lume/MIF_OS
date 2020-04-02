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
        bool LoadProgramTest_GivenNotEnoughRam_MoveMemoryToSwap();
        bool LoadProgramTest_GivenNotEnoughMemory_FailToAllocate();
        bool LoadProgramTest_GivenProgramTooBigForSinglePage_AllocateOnMultiplePages();
        bool LoadProgramTest_GivenSeveralPrograms_LoadedSuccesfully();
        bool ExecuteProgramTest_GivenLoadedProgram_ExecuteSuccesfully();
        bool ExecuteProgramTest_GivenLoadedProgramMultiPages_ExecuteSuccesfully();
        bool ExecuteProgramTest_AfterExecution_MemoryIsFreed();
        bool ExecuteProgramTest_WhenMemoryOnSwap_RetrievesMemoryFromSwap();
        bool ExecuteProgramTest_MemoryIsFreed_PagesCanBeAccessed();
        bool ExecuteProgramTest_SeveralProgramsExecuted_ExecuteSuccesfully();
};