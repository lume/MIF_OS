#include "cpu.h"
#include <stdio.h>

// RAM definition
#define RAM_SIZE 128
int RAM[RAM_SIZE] = {0};

// Register definition
int pc = 0x0;   // program counter
int addr = 0x0; // internal addr register
int ax = 0x0;   // accumulator
int ir = 0x0;   // instruction register
int sp = RAM_SIZE - 1; // stack pointer 

// Flag definition
enum
{
    sf = 1 << 0,
    zf = 1 << 1
};

// Instruction definition
enum
{
    STOP = 0,
    LOAD,
    STORE,
    ADD,
    SUB,
    JZ,
    JNZ,
    JL,
    JLE,
    JG,
    JGE,
    JMP,
    DIV,
    MOD,
    PUSH,
    POP,
    INC,
    DEC
};

void OP_STOP()
{}

void OP_LOAD()
{}

void OP_STORE()
{}

void OP_ADD()
{}

void OP_SUB()
{}

void OP_JZ()
{}

void OP_JNZ()
{}

void OP_JL()
{}

void OP_JLE()
{}

void OP_JG()
{}

void OP_JGE()
{}

void OP_JMP()
{}

void OP_DIV()
{}

void OP_MOD()
{}

void OP_PUSH()
{}

void OP_POP()
{}

void OP_INC()
{}

void OP_DEC()
{}

// public functions

int* GetRAM()
{
    return RAM;
}

void ExecuteMachineCode(char* machineCode)
{
    printf("Cpu executing machine code...");
}