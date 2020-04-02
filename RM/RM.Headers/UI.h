#pragma once
#include "cpu.h"

class UI
{
    public:
        UI(std::string programName, bool step);
        UI();
        void Update();

        Cpu cpu;

    private:
        std::string programName;

        bool step;
};