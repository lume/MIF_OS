#pragma once
#include "cpu.h"

class UI
{
    public:
        UI(bool step);
        UI(std::string programName, bool step);
        void Update();

        Cpu cpu;

    private:
        std::string programName;

        bool step;
};