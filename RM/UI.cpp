#include "UI.h"
#include <iostream>

UI::UI(std::string programName, bool step)
{
    this->cpu = cpu;
    this->programName = programName;
    this->step = step;
}

void UI::Update()
{
    if(step)
    {
        getchar();
    }
    std::cout << "\033[2J\033[1;1H";

    auto snap = cpu.SaveToSnapshot();

    std::cout << "Active program: " << this->programName << "\n";
    std::cout << "CPU snapshot: \n";
    std::cout << "pc: " << snap.pc << " ";
    std::cout << "addr: " << snap.addr << " ";
    std::cout << "acc: " << snap.acc << " ";
    std::cout << "ir: " << snap.ir << " ";
    std::cout << "sp: " << snap.sp << " ";
    std::cout << "fs: " << snap.fs << " ";
    std::cout << "xReg: " << snap.xReg << " ";
    std::cout << "cReg: " << snap.cReg << " ";
    std::cout << "retReg: " << snap.retReg << " ";
    std::cout << std::endl;

    std::cout << "RAM: ";
    this->cpu.ShowRam();
    printf("instruction-> %d\n", snap.ir); 
    
}