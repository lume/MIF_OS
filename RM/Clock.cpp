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
    if(this->step)
    {
        while(isOn)
        {
            activeProgram = cpu.ExecuteProgram(activeProgram, 1);
            this->ui.cpu = cpu;
            this->ui.Update();
            if(!((cpu.SaveToSnapshot().fs & (1 << 3)) == 0))
            {
                std::cout << "Program has finished!";
                isOn = false;
            }
        }
        this->ui.Update();
    }
    else
    {
        this->ui.Update();
        while(isOn)
        {
            activeProgram = cpu.ExecuteProgram(activeProgram, 1);
            this->ui.cpu = cpu;

            if(!((cpu.SaveToSnapshot().fs & (1 << 3)) == 0))
            {
                std::cout << "Program has finished!";

                isOn = false;
            }
        }
        this->ui.Update();
    }
}

void Clock::Start(std::string programName, UI ui)
{
    InitSwapDisk();
    this->ui = ui;
    activeProgram = cpu.LoadProgram(programName); 
    this->ui.cpu = cpu;
}

void Clock::InitSwapDisk()
{
    iocontrol.InitDisk();
}