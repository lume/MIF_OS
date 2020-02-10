#pragma once 
#include <vector>

void ShowRam(); // Show the contents of the RAM
void LoadProgram(std::vector<int> machineCode);// Load the Program machine code to the memory
void ExecuteProgram(); // Execute the loaded program
