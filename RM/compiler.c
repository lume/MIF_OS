#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char CheckIfMnemo(char* str);
char CheckIfDirct(char* str);
void ParseDirct(char* str);
void ParseMnemo(char* str);

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

// List of directives
const char* dircts[] = {"org", "label", "data", "mdata"};
int dirctLen = sizeof(dircts)/sizeof(dircts[0]);

char* CompileToMemory(char* sourceFile)
{
    char line[64];
    char* nread;
    size_t len = 64; 
    char delim[] = " ";
    char* token;
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
        //TODO strtok?
        token = strtok(nread, delim);

        printf("%s", token);
        if(CheckIfDirct(token) == 1)
            ParseDirct(nread);
        if(CheckIfMnemo(token) == 1)
            ParseMnemo(nread);
    }

    // Closing operation
    fclose(stream);
    return NULL;
}

char CheckIfMnemo(char* str)
{   
    for(int i = 0; i < mnemLen; i++)
    {
        printf("%s", mnems[i]);
    }
    return 0;
}

char CheckIfDirct(char* str)
{
    for(int i = 0; i < dirctLen; i++)
    {
        printf("%s", dircts[i]);
    }
    return 0;
}

void ParseDirct(char* str)
{}

void ParseMnemo(char* str)
{}
