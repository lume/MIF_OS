#include <stdio.h>
#include "cpu.h"
#include <iostream>
#include "IOControl.h"

//TODO: Implement memory segmentation

int main(int argc, char** argv)
{   
    if(argc == 1)
    {
        printf("no file provided");
        exit(1);
    }

    printf("Loading the program to the RAM\n");

    Cpu cpu = Cpu();
    Program program;
    try
    {
        program = cpu.LoadProgram(argv[1]);
    
        cpu.ShowRam();

        cpu.ExecuteProgram(program);

        cpu.ShowRam();

    }
    catch(const std::exception &e)
    {
        std::cout << e.what();
    }
    return 0;
}