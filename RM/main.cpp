#include <stdio.h>
#include "cpu.h"

//TODO: Write a make file

int main(int argc, char** argv)
{   
    if(argc == 1)
        printf("no file provided %d", argc);
    

    printf("Loading the program to the RAM\n");

    Cpu cpu = Cpu();

    cpu.LoadProgram("test");

    printf("RAM before execution:\n");
    cpu.ShowRam();
    printf("Executing program:\n");
    cpu.ExecuteProgram();
    printf("RAM after execution:\n");
    cpu.ShowRam();

    return 0;
}