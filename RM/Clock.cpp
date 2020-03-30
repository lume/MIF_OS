#include "Clock.h"
#include <iostream>

Clock::Clock()
{
    isOn = true;

    cpu = Cpu();
    memcontrol = Memcontrol();
    iocontrol = IOControl();
}

void Clock::Update()
{
    while(isOn)
    {
        //Run a program for 100 cycles and stop
        cpu.ShowRam();
        cpu.ExecuteProgram(activeProgram, 100);
        cpu.ShowRam();
        isOn = false;
    }
}

void Clock::Start()
{
    InitSwapDisk();
    activeProgram = cpu.LoadProgram("out.txt"); 
}

void Clock::InitSwapDisk()
{
    iocontrol.InitDisk();
}