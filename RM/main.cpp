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

    //Cpu cpu = Cpu();

    /*Program program = cpu.LoadProgram(argv[1]);
    cpu.ShowRam();

    cpu.ExecuteProgram(program);

    cpu.ShowRam();
    */
    IOControl c = IOControl();
    c.InitDisk();

    std::array<int, 4096> testArr;
    testArr[0] = 1337;
    testArr[1] = 1889;
    testArr[2] = 12333;
    testArr[3] = 322;
    testArr[4] = 1322;
    

    c.WriteSwapData(256, testArr);
    auto d = c.ReadSwapData(256);
    int z = 0;

    for(auto i : d)
    {
        if(z == 4095)
            break;
        std::cout << i << " ";
        z++;
    }

    return 0;
}