#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

char CheckIfMnemo(char* str);
char CheckIfDirct(char* str);

void ParseDirct(char* dirct, char* line);
void ParseMnemo(char* mnemo, char* line);

void compile_ORG(char* line);
void compile_LABEL(char* line);
void compile_DATA(char* line);
void compile_MDATA(char* line);

void undefinedLabel(char* label);

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

char* CompileToMemory(char* sourceFile)
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
        strcpy(temp, nread);
        token = strtok(nread, delim);
        for(int i = 0; token[i]; i++)
            token[i] = tolower(token[i]);

        if(CheckIfDirct(token) == 1)
            ParseDirct(token, temp); //TODO: Add temp string as param here
        else if(CheckIfMnemo(token) == 1)
            ParseMnemo(token, temp); // TODO: Add temp string as param here
    }

    // Closing operation
    fclose(stream);
    return NULL;
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
                undefinedLabel(jumpTarget);
                
            }
            else
            {

            }
        }
    }
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
    //TODO: Read about the possibility to refactor strtok in cpp
    char delim[] = " ";
    char* orgToken = strtok(line, delim);
    std::string paramToken = strtok(NULL, delim);

    //NOTE: Passing std::string to printf prints 0c..
    //printf("%s %s\n", orgToken, paramToken);
    //std::cout<< paramToken << std::endl;
    if(labels.find(paramToken) == labels.end())
    {
        //not found, adding label to our label list
        labels.insert({paramToken, src_counter});
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
    //TODO: read about data segments more 
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

void undefinedLabel(char* label)
{
    std::string newLabel = strcat("?", label);
    std::cout << newLabel << std::endl;
}
