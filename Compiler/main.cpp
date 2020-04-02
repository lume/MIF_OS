#include "compiler.h"
#include <iostream>

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Not enough parameters! Did you forget to pass a file to compile?" << std::endl;
        return 1;
    }
    
    CompileToFile(argv[1], argv[2]);

    return 0;    
}