#include "Clock.h"
#include <iostream>
#include <pthread.h>

Clock::Clock(Cpu cpu, bool step)
{
    isOn = true;
    this->cpu = cpu;
    memcontrol = Memcontrol();
    iocontrol = IOControl();
    this->step = step;
}

void Clock::Update()
{
    //this->ui.Update();
    while(isOn)
    {
        activeProgram = cpu.ExecuteProgram(activeProgram, 1);
        //this->ui.cpu = cpu;
        if(!((cpu.SaveToSnapshot().fs & (1 << 3)) == 0))
        {
            std::cout << "Program has finished!";

            isOn = false;
        }        
    }
    //this->ui.Update();
}

void Clock::Start()
{
    InitSwapDisk();
    //this->ui = ui;
    std::cout << "Loading bootloader...\n";
    activeProgram = cpu.LoadProgram("bootl"); 
    std::cout << "Bootloader is loaded, executing...\n";
    //this->ui.cpu = cpu;
}

void Clock::InitSwapDisk()
{
    iocontrol.InitDisk();
}