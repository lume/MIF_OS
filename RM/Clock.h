#pragma once

#include "cpu.h"
#include "memcontrol.h"
#include "IOControl.h"

// Clock is responsible for calling updates on each module
#define DEFAULT_CPU_TIME 100 //100 cycles

class Clock
{
    public:
        bool isOn;

        Clock();
        void Update();
        void Start();
    private:
        Cpu cpu;
        Memcontrol memcontrol;
        IOControl iocontrol;
        Program activeProgram;

        void InitSwapDisk();
};