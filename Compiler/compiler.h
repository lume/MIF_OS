#pragma once

#include <vector>
#include <string>

std::vector<int> CompileToMemory(char* sourceFile);
void CompileToFile(char* sourceFile, std::string output = "out.txt");