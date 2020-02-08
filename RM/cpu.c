#include "cpu.h"
#include <stdint.h>
#include <stdio.h>

// RAM definition
#define RAM_SIZE 128
uint16_t RAM[RAM_SIZE] = {0};

// Register definition
uint16_t pc = 0x0;   // program counter
uint16_t addr = 0x0; // internal addr register
uint16_t acc = 0x0;   // accumulator
uint16_t ir = 0x0;   // instruction register
uint16_t sp = RAM_SIZE - 1; // stack pointer 
uint16_t fs = 0x0;

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
{
    exit(0);
}

void OP_LOAD()
{
    pc++;
    addr = RAM[pc];
    acc = RAM[addr];
    pc++;
}

void OP_STORE()
{
    pc++;
    addr = RAM[pc];
    RAM[addr] = acc;
    pc++;
}

void OP_ADD()
{
    pc++;
    addr = RAM[pc];
    acc += RAM[pc];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void OP_SUB()
{
    pc++;
    addr = RAM[pc];
    acc -= RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void OP_JZ()
{
    pc++;
    if(zf)
    {
        int offset = RAM[pc];
        pc++;
        pc += offset;
    }
    else
    {
        pc++;
    }
}

void OP_JNZ()
{
    pc++;
    if(!zf)
    {
        int offset = RAM[pc];
        pc++;
        pc += offset;
    }
    else
    {
        pc++;
    }
}

void OP_JL()
{
    pc++;
    if(sf)
    {
        int offset = RAM[pc];
        pc++;
        pc += offset;
    }
    else
    {
        pc++;
    }
}

void OP_JLE()
{
    pc++;
    if(sf || zf)
    {
        int offset = RAM[pc];
        pc++;
        pc += offset;
    }
    else
    {
        pc++;
    }
}

void OP_JG()
{
    pc++;
    if(!sf)
    {
        int offset = RAM[pc];
        pc++;
        pc += offset;
    }
    else
    {
        pc++;
    } 
}

void OP_JGE()
{
    pc++;
    if(!sf || zf)
    {
        int offset = RAM[pc];
        pc++;
        pc += offset;
    }
    else
    {
        pc++;
    }
}

void OP_JMP()
{
    pc++;
    int offset = RAM[pc];
    pc++;
    pc += offset;
}

void OP_DIV()
{
    pc++;
    addr = RAM[pc];
    acc = acc / RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void OP_MOD()
{
    pc++;
    addr = RAM[pc];
    acc = acc / RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void OP_PUSH()
{
    pc++;
    addr = sp;
    RAM[addr] = acc;
    sp -= 1;
}

void OP_POP()
{
    pc++;
    sp++;
    addr = sp;
    acc = RAM[addr];
}

void OP_INC()
{
    pc++;
    acc++;
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
}

void OP_DEC()
{
    pc++;
    acc--;
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
}

void UNDEFINED()
{
    printf("UNDEFINED INSTRUCTION CODE");
    exit(0);
}
// get the next instruction
void Fetch()
{
    ir = RAM[pc];
    printf("instruction-> %d", ir); // printing to make better understanding of what's going on
    Decode(ir);
}

// decode the instruction 
void Decode(uint16_t instruction)
{
    switch (instruction)
    {
    case(STOP):
        OP_STOP();
        break;
    case(LOAD):
        OP_LOAD();
        break;
    case(STORE):
        OP_STORE();
        break;
    case(ADD):
        OP_ADD();
        break;
    case(SUB):
        OP_SUB();
        break;
    case(JZ):
        OP_JZ();
        break;
    case(JNZ):
        OP_JNZ();
        break;
    case(JL):
        OP_JL();
        break;
    case(JLE):
        OP_JLE();
        break;
    case(JG):
        OP_JG();
        break;
    case(JGE):
        OP_JGE();
        break;
    case(JMP):
        OP_JMP();
        break;
    case(DIV):
        OP_DIV();
        break;
    case(MOD):
        OP_MOD();
        break;
    case(PUSH):
        OP_PUSH();
        break;
    case(POP):
        OP_POP();
        break;
    case(INC):
        OP_INC();
        break;
    case(DEC):
        OP_DEC();
        break;
    default:
        UNDEFINED();
        break;
    }
}

// public functions

void ShowRam()
{
    printf("Showing what's inside of RAM:\n");

    printf("[");
    for(int i = 0; i < RAM_SIZE; i++)
    {
        printf("%d, ", RAM[i]);
    }
    printf("]");
}

void LoadProgram(char* machineCode)
{}

void ExecuteProgram()
{}
