#include "cpu.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <array>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>

// Flag definition
enum
{
    sf = 1 << 0,
    zf = 1 << 1,
    lf = 1 << 2
};

// Instruction definition
enum
{
    STOP = 0,
    LOADA,
    LOADI,
    LOADR,
    STOREA,
    STORER,
    ADDA,
    ADDI,
    ADDR,
    SUBA,
    SUBI,
    SUBR,
    MULA,
    MULI,
    MULR,
    DIVA,
    DIVI,
    DIVR,
    JZ,
    JNZ,
    JL,
    JLE,
    JG,
    JGE,
    JMP,
    MOD,
    PUSH,
    POP,
    INC,
    DEC,
    SHL,
    SHR,
    INT,
    ANDA,
    ANDI,
    ANDR,
    ORA,
    ORI,
    ORR,
    XORA,
    XORI,
    XORR,
    CMPA,
    CMPI,
    CMPR,
    CALL
};

void Cpu::OP_STOP()
{
    exit(0);
}

void Cpu::OP_LOADA()
{
    pc++;
    addr = RAM[pc];
    acc = RAM[addr];
    pc++;
}

void Cpu::OP_STOREA()
{
    pc++;
    addr = RAM[pc];
    RAM[addr] = acc;
    pc++;
}

void Cpu::OP_ADDA()
{
    pc++;
    addr = RAM[pc];
    acc += RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_ADDI()
{
    pc++;
    acc += RAM[pc];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_SUBA()
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

void Cpu::OP_JZ()
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

void Cpu::OP_JNZ()
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

void Cpu::OP_JL()
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

void Cpu::OP_JLE()
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

void Cpu::OP_JG()
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

void Cpu::OP_JGE()
{
    pc++;
    if(!sf || zf)
    {
            //TODO: Add a check for overflow.
        int offset = RAM[pc]; // TODO: replace int with uint8 for jumps, reserve uint16 for call
        pc++;
        pc += offset;
    }
    else
    {
        pc++;
    }
}

void Cpu::OP_JMP()
{
    pc++;
    int offset = RAM[pc];
    pc++;
    pc += offset;
}

void Cpu::OP_DIVA()
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

void Cpu::OP_MOD()
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

void Cpu::OP_PUSH()
{
    pc++;
    addr = sp;
    RAM[addr] = acc;
    sp -= 1;
}

void Cpu::OP_POP()
{
    pc++;
    sp++;
    addr = sp;
    acc = RAM[addr];
}

void Cpu::OP_INC()
{
    pc++;
    acc++;
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
}

void Cpu::OP_DEC()
{
    pc++;
    acc--;
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
}

void Cpu::UNDEFINED()
{
    printf("UNDEFINED INSTRUCTION CODE");
    exit(0);
}

// get the next instruction
void Cpu::Fetch()
{
    ir = RAM[pc];
    printf("instruction-> %d\n", ir); // printing to make better understanding of what's going on
}

// decode the instruction 
void Cpu::Decode()
{
    switch (ir)
    {
    case(STOP):
        OP_STOP();
        break;
    case(LOADA):
        OP_LOADA();
        break;
    case(LOADI):
        OP_LOADI();
        break;
    case(LOADR):
        OP_LOADR();
        break;
    case(STOREA):
        OP_STOREA();
        break;
    case(STORER):
        OP_STORER();
        break;
    case(ADDA):
        OP_ADDA();
        break;
    case(ADDI):
        OP_ADDI();
        break;
    case(ADDR):
        OP_ADDR();
        break;
    case(SUBA):
        OP_SUBA();
        break;
    case(SUBI):
        OP_SUBI();
        break;
    case(SUBR):
        OP_SUBR();
        break;
    case(MULA):
        OP_MULA();
        break;
    case(MULI):
        OP_MULI();
        break;
    case(MULR):
        OP_MULR();
        break;
    case(DIVA):
        OP_DIVA();
        break;
    //TODO: Continue here
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

void Cpu::ShowRam()
{
    printf("[");
    for(int i = 0; i < RAM_SIZE; i++)
    {
        printf("%d, ", RAM[i]);
    }
    printf("]");

    if(zf)
        printf("zero flag is set!\n");
    if(sf)
        printf("sign flag is set!\n");
}

void Cpu::LoadProgram(std::string filename)
{ 
    //TODO: Rework program loading so it handles virtual memory correctly
    //For now we do not allow to have programs bigger than our RAM
    std::vector<int> machineCode;
    std::stringstream tempStream;
    std::ifstream ifile(filename);

    if(ifile)
    {
        tempStream << ifile.rdbuf();
        ifile.close();
        char* tempString = strdup(tempStream.str().c_str());
        char* ch = strtok(tempString, " ");

        while(ch != NULL)
        {
            machineCode.push_back(atoi(ch));
            ch = strtok(NULL, " ");
            printf("%c", ch);
        }
    }
    else
    {
        printf("Failed to a program to launch: %s", strerror(errno));
        exit(1);
    }
    

    printf("\n");

    for(int i = 0; i < machineCode.size(); i++)
    {
        printf("%d", machineCode[i]);
    }

    if(machineCode.size() > RAM_SIZE)
    {
        printf("Not enough RAM space for this program!");
        exit(1);
    }
    else
    {
        for(int i = 0; i < machineCode.size(); i++)
        {
            RAM[i] = machineCode[i];
        }
    }
}

void Cpu::ExecuteProgram()
{
    while(RAM[pc] != 0)
    {
        Fetch();
        Decode();
    }
}
