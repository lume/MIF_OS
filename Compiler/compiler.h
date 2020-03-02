#pragma once

#include <vector>
#include <string>

std::vector<int> CompileToMemory(char* sourceFile);
//TODO:  Add  compilation to file
void CompileToFile(char* sourceFile, std::string output = "out.txt");