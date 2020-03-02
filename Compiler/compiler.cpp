#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <map>
//#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

char CheckIfMnemo(char* str);
char CheckIfDirct(char* str);

void ParseDirct(char* dirct, char* line);
void ParseMnemo(char* mnemo, char* line);

void compile_ORG(char* line);
void compile_LABEL(char* line);
void compile_DATA(char* line);
void compile_MDATA(char* line);

int getOpcode(char* mnem);
int getOpSize(char* mnem);

std::string stripString(char* str);

std::string undefinedLabel(char* label);

// Opcodes
#define STOP_OPC 0
#define LOAD_OPC 1
#define STORE_OPC 2
#define ADD_OPC 3
#define SUB_OPC 4
#define JZ_OPC 5
#define JNZ_OPC 6
#define JL_OPC 7
#define JLE_OPC 8
#define JG_OPC 9
#define JGE_OPC 10
#define JMP_OPC 11
#define DIV_OPC 12
#define MOD_OPC 13
#define PUSH_OPC 14
#define POP_OPC 15
#define INC_OPC 16
#define DEC_OPC 17

// Instruction sizes
#define STOP_SIZE 1
#define LOAD_SIZE 2
#define STORE_SIZE 2
#define ADD_SIZE 2
#define SUB_SIZE 2
#define JZ_SIZE 2
#define JNZ_SIZE 2
#define JL_SIZE 2
#define JLE_SIZE 2
#define JG_SIZE 2
#define JGE_SIZE 2
#define JMP_SIZE 2
#define DIV_SIZE 2
#define MOD_SIZE 2
#define PUSH_SIZE 1
#define POP_SIZE 1
#define INC_SIZE 1
#define DEC_SIZE 1

// List of mnemonics
const char* mnems[] = {"stop", "load", "store", "add", "sub", "jz", "jnz", "jl", 
"jle", "jg", "jge", "jmp", "div", "mod", "push", "pop", "inc", "dec"};
int mnemLen = sizeof(mnems)/sizeof(mnems[0]);

const char* jumps[] = {"jz", "jnz", "jl", "jle", "jg", "jge", "jmp"};
int jmplen = sizeof(jumps)/sizeof(jumps[0]);

// List of directives
const char* dircts[] = {"org", "label", "data", "mdata"};
int dirctLen = sizeof(dircts)/sizeof(dircts[0]);

// Compiler config
uint32_t src_counter = 0; // position from which the code starts
std::vector<int> code;  // code segment
std::map<std::string, int> labels;

std::vector<int> CompileToMemory(char* sourceFile)
{
    char line[64];
    char* nread;
    size_t len = 64; 
    char delim[] = " ";
    char* token;

    char temp[64];
    // 1. Getting the assembly code source file
    FILE *stream = fopen(sourceFile, "r");
    if(stream == NULL)
    {
        printf("Source file could not be opened for compilation.\n");
        exit(1);
    }

    // 2. Parsing the assembly code line-by-line
    while((nread = fgets(line, len, stream)) != NULL)
    {
        if(strcmp(nread, "") == 0 || strcmp(nread, "\n") == 0) 
            continue;

        printf("\n%s\n", nread);
        if(nread[strlen(nread)-1] == '\n')
        {
            nread[strlen(nread)-1] = '\0';
        }

        strcpy(temp, nread);
        token = strtok(nread, delim);

        for(int i = 0; token[i]; i++)
            token[i] = tolower(token[i]);

        if(CheckIfDirct(token) == 1)
            ParseDirct(token, temp); 
        else if(CheckIfMnemo(token) == 1)
            ParseMnemo(token, temp); 
    }

    // Closing operation
    fclose(stream);
    printf("\n\nFinal compilation result: \n");
    printf("[");
        std::for_each(code.begin(), code.end(),[](int i){
            printf("%d,", i);
        });
    printf("]");
    return code;
}

void CompileToFile(char* sourceFile, std::string output)
{
    std::vector<int> machineCode = CompileToMemory(sourceFile);

    std::ofstream ofile(output);

    std::stringstream machineCodeString;
    std::copy(machineCode.begin(), machineCode.end(), std::ostream_iterator<int>(machineCodeString, ""));

    ofile << machineCodeString.str().c_str();
    ofile.close();
}

char CheckIfMnemo(char* str)
{   
    for(int i = 0; i < mnemLen; i++)
    {
        if(strcmp(str, mnems[i]) == 0)
            return 1;
    }
    return 0;
}

char CheckIfDirct(char* str)
{
    for(int i = 0; i < dirctLen; i++)
    {
       if(strcmp(str, dircts[i]) == 0)
            return 1;
    }
    return 0;
}

void ParseDirct(char* dirct, char* line)
{
    //printf("Parsing dirct: %s\n", dirct);
    if(strcmp(dirct, "org") == 0)
    {
        compile_ORG(line);
    }
    if(strcmp(dirct, "label") == 0)
    {
        compile_LABEL(line);
    }
    if(strcmp(dirct, "data") == 0)
    {
        compile_DATA(line);
    }
    if(strcmp(dirct, "mdata") == 0)
    {
        compile_MDATA(line);
    }
}

void ParseMnemo(char* mnemo, char* line)
{
    //printf("Parsing mnemo: %s\n", mnemo);
    char delim[] = " ";
    char* mnemToken = strtok(line, delim);

    for(int i = 0; i < jmplen; i++)
    {
        if(strcmp(mnemToken, jumps[i]) == 0)
        {
            //it is a jump, now check if label is correct
            char* jumpTarget = strtok(NULL, delim);
            if(labels.find(jumpTarget) == labels.end())
            {
                //not found
                std::string undefLabel = undefinedLabel(jumpTarget);
                std::cout << undefLabel << std::endl;
                
                //insert jump instruction into code
                int opcode = getOpcode(mnemToken);
                code.insert(code.end(), opcode);

                //insert undefined label into code
                for(int i = 0; i < undefLabel.length(); i++)
                {
                    code.insert(code.end(), undefLabel[i]);
                }

                printf("[");
                std::for_each(code.begin(), code.end(),[](int i){
                    printf("%d,", i);
                });
                printf("]");
            }
            else
            {
                //insert jump instruction into code
                int opcode = getOpcode(mnemToken);
                code.insert(code.end(), opcode);

                int counterDiff = labels.at(jumpTarget) - (src_counter + 2); // This line may throw an exception
                code.insert(code.end(), opcode);
                code.insert(code.end(), counterDiff);
            }
            int opsize = getOpSize(mnemToken);
            src_counter += opsize;
            return;
        }
    }

    // This is not a jump
    int opcode = getOpcode(mnemToken);
    code.insert(code.end(), opcode);
    int opsize = getOpSize(mnemToken);
    if(opsize > 1)
    {
        char* arg = strtok(NULL, delim);
        code.insert(code.end(), atoi(arg));
    }
    src_counter += opsize;

     printf("[");
        std::for_each(code.begin(), code.end(),[](int i){
            printf("%d,", i);
        });
    printf("]");
    return;
}   

void compile_ORG(char* line)
{
    char delim[] = " ";
    char* orgToken = strtok(line, delim);
    uint16_t paramToken = atoi(strtok(NULL, delim));

    printf("%s %d\n", orgToken, paramToken);

    if(src_counter > paramToken)
    {
        printf("Error when compiling org %d: counter value is incorrect", paramToken);
        exit(1);
    }
    else if(src_counter < paramToken)
    {
        // Probably will need to modify this function here ...
        src_counter = paramToken;
    }
}

void compile_LABEL(char* line)
{
    char delim[] = " ";
    char* orgToken = strtok(line, delim);
    std::string paramToken = strtok(NULL, delim);

    if(labels.find(paramToken) == labels.end())
    {
        //not found, adding label to our label list
        labels.insert({paramToken, src_counter});

        for(auto it = labels.cbegin(); it != labels.cend(); ++it)
        {
            std::cout << it->first << " " << it->second << std::endl; 
        }
    }
    else
    {
        //such label already exists!
        printf("label '%s' already defined!", paramToken);
        exit(1);
    }
}

void compile_DATA(char* line)
{
    char delim[] = " ";
    char* orgToken = strtok(line, delim);
    std::string paramToken = strtok(NULL, delim);

    for(int i = 0; i < paramToken.length(); i++)
    {
        if(paramToken[i] != '\n')
            code.insert(code.end(), (int)paramToken[i] -0x30);
    }

    printf("[");
    std::for_each(code.begin(), code.end(),[](int i){
        printf("%d,", i);
    });
    printf("]");
}

void compile_MDATA(char* line)
{
    // Won't be implementing this for now
    printf("mdata is not implemented");
}

std::string undefinedLabel(char* label)
{
    std::string newLabel = "?";
    newLabel += label;
    return newLabel;
}

int getOpcode(char* mnem)
{
    if(strcmp(mnem, "stop") == 0)
        return STOP_OPC;
    if(strcmp(mnem, "load") == 0)
        return LOAD_OPC;
    if(strcmp(mnem, "store") == 0)
        return STORE_OPC;
    if(strcmp(mnem, "add") == 0)
        return ADD_OPC;
    if(strcmp(mnem, "sub") == 0)
        return SUB_OPC;
    if(strcmp(mnem, "jz") == 0)
        return JZ_OPC;
    if(strcmp(mnem, "jnz") == 0)
        return JNZ_OPC;
    if(strcmp(mnem, "jl") == 0)
        return JL_OPC;
    if(strcmp(mnem, "jle") == 0)
        return JLE_OPC;
    if(strcmp(mnem, "jg") == 0)
        return JG_OPC;
    if(strcmp(mnem, "jge") == 0)
        return JGE_OPC;
    if(strcmp(mnem, "jmp") == 0)
        return JMP_OPC;
    if(strcmp(mnem, "div") == 0)
        return DIV_OPC;
    if(strcmp(mnem, "mod") == 0)
        return MOD_OPC;
    if(strcmp(mnem, "push") == 0)
        return PUSH_OPC;
    if(strcmp(mnem, "pop") == 0)
        return POP_OPC;
    if(strcmp(mnem, "inc") == 0)
        return INC_OPC;
    if(strcmp(mnem, "dec") == 0)
        return DEC_OPC;
    return 999;
}

int getOpSize(char* mnem)
{
    if(strcmp(mnem, "stop") == 0)
        return STOP_SIZE;
    if(strcmp(mnem, "load") == 0)
        return LOAD_SIZE;
    if(strcmp(mnem, "store") == 0)
        return STORE_SIZE;
    if(strcmp(mnem, "add") == 0)
        return ADD_SIZE;
    if(strcmp(mnem, "sub") == 0)
        return SUB_SIZE;
    if(strcmp(mnem, "jz") == 0)
        return JZ_SIZE;
    if(strcmp(mnem, "jnz") == 0)
        return JNZ_SIZE;
    if(strcmp(mnem, "jl") == 0)
        return JL_SIZE;
    if(strcmp(mnem, "jle") == 0)
        return JLE_SIZE;
    if(strcmp(mnem, "jg") == 0)
        return JG_SIZE;
    if(strcmp(mnem, "jge") == 0)
        return JGE_SIZE;
    if(strcmp(mnem, "jmp") == 0)
        return JMP_SIZE;
    if(strcmp(mnem, "div") == 0)
        return DIV_SIZE;
    if(strcmp(mnem, "mod") == 0)
        return MOD_SIZE;
    if(strcmp(mnem, "push") == 0)
        return PUSH_SIZE;
    if(strcmp(mnem, "pop") == 0)
        return POP_SIZE;
    if(strcmp(mnem, "inc") == 0)
        return INC_SIZE;
    if(strcmp(mnem, "dec") == 0)
        return DEC_SIZE;
    return 999;
}