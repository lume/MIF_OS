#include <stdio.h>
#include "cpu.h"

//TODO: Write a make file
//TODO: Implement memory segmentation

//FIXME: only VAR mnem breaks on breakpoint
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
    cpu.ShowRam();

    cpu.ExecuteProgram(program);

    cpu.ShowRam();
    

    return 0;
}