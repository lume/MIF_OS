#include <stdio.h>
#include <iostream>
#include "Clock.h"
#include "cpu.h"
#include <string.h>

int main(int argc, char** argv)
{   
    bool step = false;

    if(argc < 2)
    {
        //std::cout << "Please provide a program to launch and add 'step' if you want to run the program in debug mode";
        //exit(1);
    }
    else if(argc > 2)
    {
        if(strcmp(argv[1], "step") == 0)
            step = true;
    }


    if(step)
    {
        std::cout << "Press any key to start...";
    }

    Cpu cpu = Cpu();

    Clock clock = Clock(cpu, step);

    clock.Start();
    clock.Update();

    return 0;
}