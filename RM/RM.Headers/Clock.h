#pragma once

#include "cpu.h"
#include "memcontrol.h"
#include "IOControl.h"
#include "UI.h"

class Clock
{
    public:
        bool isOn;

        Clock(Cpu cpu, bool step);
        void Update();
        void Start();
    private:
        Cpu cpu;
        Memcontrol memcontrol;
        IOControl iocontrol;
        Program activeProgram;

        void InitSwapDisk();
       // UI ui = UI();
        bool step;
};