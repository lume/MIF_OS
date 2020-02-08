#include <stdio.h>
#include "cpu.h"

//TODO: Implement the compiler
//TODO: Write a make file

int main()
{
    ExecuteMachineCode("1010");
    ShowRam();
    return 0;
}