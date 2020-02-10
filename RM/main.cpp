#include <stdio.h>
#include "cpu.h"
#include "compiler.h"

//TODO: Implement the compiler
//TODO: Write a make file

int main(int argc, char** argv)
{   
    if(argc == 1)
        printf("no file provided %d", argc);
    
    printf("Compiling %s", argv[1]);
    std::vector<int> machineCode = CompileToMemory(argv[1]);

    return 0;
}