#include <stdio.h>
#include "cpu.h"

#include "IOControl.h"

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

    //Cpu cpu = Cpu();

    /*Program program = cpu.LoadProgram(argv[1]);
    cpu.ShowRam();

    cpu.ExecuteProgram(program);

    cpu.ShowRam();
    */
    IOControl c = IOControl();
    c.InitDisk();

    std::array<int, 4096> testArr;
    testArr[0] = 1;
    testArr[1] = 1;
    testArr[2] = 1;
    testArr[3] = 1;
    testArr[4] = 1;
    

    c.WriteData(256, testArr);

    return 0;
}