#include <stdio.h>
#include <iostream>
#include "Clock.h"

//TODO: Implement memory segmentation

int main(int argc, char** argv)
{   
    Clock clock = Clock();
    clock.Start();
    clock.Update();
    
    return 0;
}