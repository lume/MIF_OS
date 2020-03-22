#include <stdio.h>
#include "cpu.h"

//TODO: Write a make file
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

    Program program = cpu.LoadProgram(argv[1]);

    printf("RAM before execution:\n");
    cpu.ShowRam();
    printf("Executing program:\n");
    cpu.ExecuteProgram(program);
    printf("RAM after execution:\n");
    cpu.ShowRam();

    return 0;
}