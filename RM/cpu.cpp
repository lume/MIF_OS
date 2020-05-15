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
#include <iostream>

//#define RAM memcontroller.RAM


// Flag definition
enum
{
    sf = 1 << 0, // sign
    zf = 1 << 1, // zero
    lf = 1 << 2, // lower
    ef = 1 << 3, // end
    of = 1 << 4, // overflow
    cf = 1 << 5, // carry
    pf = 1 << 6  // parity
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
    CALL,
    VAR,
    PTR,
    LOADV,
    LOADP,
    STOREP,
    STOREV,
    JO,
    JP,
    JC,
    RET,
    STR,
    RSTR,
    DELSTR
};

void Cpu::OP_STOP()
{
    fs |= ef;
}

void Cpu::OP_STR()
{
    std::vector<char> contents;
    char c = -1;
    pc++;
    while(c != 0)
    {
        c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        contents.insert(contents.end(), c);
        pc++;
    }
    auto memBlock = memcontroller.HeapAlloc(activeProgram, contents.size());
    std::string str(contents.begin(), contents.end());
    memcontroller.StoreStringInHeap(memBlock, str);
    acc = memBlock.start;
    //pc++;
}

void Cpu::OP_RSTR()
{

}

void Cpu::OP_DELSTR()
{
    for(auto i : HeapBlockHandlers)
    {
        if(i.start == acc)
        {
            memcontroller.HeapFree(&i);
        }
    }
}

void Cpu::OP_LOADA()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    acc = RAM[addr];
    pc++;
}

void Cpu::OP_LOADI()
{
    pc++;
    int varVal = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    acc = varVal;
    pc++;
}

void Cpu::OP_RET()
{
    if(retReg != -1)
        pc = retReg;
    retReg = -1;
}

void Cpu::OP_LOADR()
{
    pc++;
    char c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    
    if(c == 'x')
        acc = xReg;
    else if(c == 'c')
        acc = cReg;
   
    pc++;
}

void Cpu::OP_STOREA()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    RAM[addr] = acc;
    pc++;
}

void Cpu::OP_STORER()
{
    pc++;
    char c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];

    if(c == 'x')
        xReg = acc;
    else if(c == 'c')
        cReg = acc;

    pc++;
}

void Cpu::OP_ADDA()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    acc = AddInternal(RAM[addr], acc);
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_ADDI()
{
    pc++;
    acc = AddInternal(RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])]-48, acc);
    
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_ADDR()
{
    pc++;  
    char c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];

    if(c == 'x')
        acc = AddInternal(xReg, acc);
    else if(c == 'c')
        acc = AddInternal(cReg, acc);

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_SUBA()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    acc -= RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_SUBI()
{
    pc++;
    acc += RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])]-48;

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_SUBR()
{
    pc++;
    char c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];

    if(c == 'x')
        acc -= xReg;
    else if(c == 'c')
        acc -= cReg;

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_MULA()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    acc = MulInternal(RAM[addr], acc);

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_MULI()
{
    pc++;
    acc = MulInternal(RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])]-48, acc);
    
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_MULR()
{
    pc++;
    char c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];

    if(c == 'x')
        acc = MulInternal(xReg, acc);
    else if(c == 'c')
        acc = MulInternal(cReg, acc);
    
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_DIVA()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    acc = acc / RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_DIVI()
{
    pc++;
    acc = acc / (RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])]-48);

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_DIVR()
{
    pc++;
    char c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];

    if(c == 'x')
        acc /= xReg;
    else if(c == 'c')
        acc /= cReg;
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_JZ()
{
    pc++;
    if(zf)
    {
        uint8_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        pc++;
        pc = offset;
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
        uint8_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        pc++;
        pc = offset;
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
        uint8_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        pc++;
        pc = offset;
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
        uint8_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        pc++;
        pc = offset;
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
        uint8_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        pc++;
        pc = offset;
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
        uint8_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        pc++;
        pc = offset;
    }
    else
    {
        pc++;
    }
}

void Cpu::OP_JO()
{
    pc++;
    if(of)
    {
        uint8_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        pc++;
        pc = offset;
    }
    else
    {
        pc++;
    }
}

void Cpu::OP_JP()
{
    pc++;
    if(pf)
    {
        uint8_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        pc++;
        pc = offset;
    }
    else
    {
        pc++;
    }
}

void Cpu::OP_JC()
{
    pc++;
    if(cf)
    {
        uint8_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        pc++;
        pc = offset;
    }
    else
    {
        pc++;
    }
}

void Cpu::OP_JMP()
{
    pc++;
    uint8_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    pc++;
    pc = offset;
}

void Cpu::OP_MOD()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    acc = acc / RAM[memcontroller.ConvertToPhysAddress(addr)];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_PUSH()
{
    pc++;
    int a = memcontroller.ConvertToPhysAddress(sp);
    addr = a;
    RAM[addr] = acc;
    sp -= 1;
}

void Cpu::OP_POP()
{
    pc++;
    sp++;
    addr = sp;
    acc = RAM[memcontroller.ConvertToPhysAddress(addr)];
}

void Cpu::OP_INC()
{
    pc++;
    acc++;
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
}

void Cpu::OP_DEC()
{
    pc++;
    acc--;
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
}

void Cpu::OP_INT()
{
    // TODO: Implement mapping to int handlers
    pc++;
    acc = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    switch(acc)
    {
        case 1:
            break;
        case 3:
            int3(); // fork process
            break;
        case 10:
            int10(); // print
            break;
        default:
            throw std::runtime_error("Bad interrupt");
    }
    pc++;
}

void Cpu::OP_ANDA()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    acc = acc & RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_ANDI()
{
    pc++;
    acc = acc & RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_ANDR()
{
    pc++;
    char c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];

    if(c == 'x')
        xReg &= acc;
    else if(c == 'c')
        cReg &= acc;

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_ORA()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    acc = acc | RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_ORI()
{
    pc++;
    acc = acc | RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_ORR()
{
    pc++;
    char c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];

    if(c == 'x')
        xReg |= acc;
    else if(c == 'c')
        cReg |= acc;

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_XORA()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    acc = acc ^ RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_XORI()
{
    pc++;
    acc = acc ^ RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_XORR()
{
    pc++;
    char c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];

    if(c == 'x')
        xReg ^= acc;
    else if(c == 'c')
        cReg ^= acc;

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_CMPA()
{
    pc++;
    addr = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    uint16_t val = RAM[addr];
    
    if(acc < val)
        fs |= lf;
    if(acc == val)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_CMPI()
{
    pc++;
    uint16_t val = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    
    if(acc < val)
        fs |= lf;
    if(acc == val)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_CMPR()
{
    pc++;
    char c = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    uint16_t val;
    
    if(c == 'x')
        val = xReg;
    else if(c == 'c')
        val = cReg;

    if(acc < val)
        fs |= lf;
    if(acc == val)
        fs |= zf;
    if(ParityCheck())
        fs |= pf;
    pc++;
}

void Cpu::OP_CALL()
{
    pc++;
    uint16_t offset = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    pc++;
    retReg = pc;
    pc = offset;
}

void Cpu::OP_SHR()
{
    pc++;
    acc >> RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    pc++;
}

void Cpu::OP_SHL()
{
    pc++;
    acc << RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    pc++;
}

void Cpu::OP_VAR()
{
    pc++;
    int x = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
    int oldVarAddr = memcontroller.GetVarAddrIfExists(activeProgram, x);
    
    if(oldVarAddr != -1)
    {
        pc++;
    }
    else{
        int varAddr = activeProgram.dataSegment.writePointer;
        RAM[varAddr] = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        RAM[varAddr+1] = RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])];
        pc++;
        activeProgram.dataSegment.writePointer += 2; 
    }
}

void Cpu::OP_PTR()
{
    pc++;
    pc++;
}

void Cpu::OP_LOADP()
{
    pc++;
    pc++;
}

void Cpu::OP_LOADV()
{
    pc++;
    int addr = memcontroller.FindVarAddress(activeProgram, RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])]);
    acc = RAM[addr+1];
    pc++;
}

void Cpu::OP_STOREV()
{
    pc++;
    int addr = memcontroller.FindVarAddress(activeProgram, RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc])]);
    RAM[addr+1] = acc;
    pc++;
}

void Cpu::OP_STOREP()
{
    pc++;
    pc++;    
}

void Cpu::UNDEFINED()
{
    printf("UNDEFINED INSTRUCTION CODE");
    exit(0);
}

// get the next instruction
void Cpu::Fetch()
{
    int nextAddr = memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[pc]);
    ir = RAM[nextAddr];
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
    case(DIVI):
        OP_DIVI();
        break;
    case(DIVR):
        OP_DIVR();
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
    case(SHL):
        OP_SHL();
        break;
    case(SHR):
        OP_SHR();
        break;
    case(INT):
        OP_INT();
        break;
    case(ANDA):
        OP_ANDA();
        break;
    case(ANDI):
        OP_ANDI();
        break;
    case(ANDR):
        OP_ANDR();
        break;
    case(ORA):
        OP_ORA();
        break;
    case(ORI):
        OP_ORI();
        break;
    case(ORR):
        OP_ORR();
        break;
    case(XORA):
        OP_XORA();
        break;
    case(XORI):
        OP_XORI();
        break;
    case(XORR):
        OP_XORR();
        break;
    case(CMPA):
        OP_CMPA();
        break;
    case(CMPI):
        OP_CMPI();
        break;
    case(CMPR):
        OP_CMPR();
        break;
    case(CALL):
        OP_CALL();
        break;
    case(VAR):
        OP_VAR();
        break;
    case(PTR):
        OP_PTR();
        break;
    case(STOREV):
        OP_STOREV();
        break;
    case(LOADV):
        OP_LOADV();
        break;
    case(LOADP):
        OP_LOADP();
        break;
    case(STOREP):
        OP_STOREP();
        break;
    case(JO):
        OP_JO();
        break;
    case(JC):
        OP_JC();
        break;
    case(JP):
        OP_JP();
        break;
    case(RET):
        OP_RET();
        break;
    case(STR):
        OP_STR();
        break;
    case(RSTR):
        OP_RSTR();
        break;
    case(DELSTR):
        OP_DELSTR();
        break;
    default:
        UNDEFINED();
        break;
    }
}

// public functions

void Cpu::ShowRam()
{
    printf("CODE: (starting from %d)\n", memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[0]));
    printf("[");
    for(int i = 0; i < 50; i++)
    {
        printf("%d, ", RAM[memcontroller.ConvertToPhysAddress(activeProgram.codeSegment.memory.addresses[i])]);
    }
    printf("]\n");

    printf("STACK: (starting from %d)\n", memcontroller.ConvertToPhysAddress(activeProgram.stackSegment.memory.addresses[activeProgram.stackSegment.memory.addresses.size()-1]));
    printf("[");
    for(int i = activeProgram.stackSegment.memory.addresses.size(); 
    i > activeProgram.stackSegment.memory.addresses.size() - 50; i--)
    {
        printf("%d, ", RAM[memcontroller.ConvertToPhysAddress(activeProgram.stackSegment.memory.addresses[i])]);
    }
    printf("]\n");

    printf("DATA:(starting from %d)\n", memcontroller.ConvertToPhysAddress(activeProgram.dataSegment.memory.addresses[0]));
    printf("[");
    for(int i = 0; i < 50; i++)
    {
        printf("%d, ", RAM[memcontroller.ConvertToPhysAddress(activeProgram.dataSegment.memory.addresses[i])]);
    }
    printf("]\n");
}

Program Cpu::LoadProgram(std::vector<int> programCode)
{
    // Machine code is loaded to the list, now we load this code into RAM
    Segment codeSegment;
    Segment stackSegment;
    Segment dataSegment;

    codeSegment = memcontroller.InitSegment(0);
    dataSegment = memcontroller.InitSegment(0);
    stackSegment = memcontroller.InitSegment(1);
    std::vector<int> pagesToIgnore;

    for(int i = 0; i < programCode.size(); i++){
        if(i >= codeSegment.memory.addresses.size())
        {
            pagesToIgnore.insert(pagesToIgnore.end(), dataSegment.memory.usedPages.begin(),
            dataSegment.memory.usedPages.end());

            pagesToIgnore.insert(pagesToIgnore.end(), codeSegment.memory.usedPages.begin(),
            codeSegment.memory.usedPages.end());

            pagesToIgnore.insert(pagesToIgnore.end(), stackSegment.memory.usedPages.begin(),
            stackSegment.memory.usedPages.end());

            Memory newmem = memcontroller.AllocateMemory(programCode.size() - codeSegment.memory.addresses.size(), pagesToIgnore);

            codeSegment.memory.usedPages.insert(codeSegment.memory.usedPages.end(),
            newmem.usedPages.begin(), newmem.usedPages.end());
            
            codeSegment.memory.addresses.insert(codeSegment.memory.addresses.end(),
            newmem.addresses.begin(), newmem.addresses.end());
        }
        memcontroller.WriteSegment(codeSegment, codeSegment.memory.addresses[i], programCode[i]);
    }

    sp = stackSegment.startPointer + PAGE_SIZE-1;
    pc = 0;

    activeProgram = {dataSegment, codeSegment, stackSegment, {pc, 0, 0, 0, sp, 0, 0}};

    return activeProgram;
}

Program Cpu::LoadProgram(std::string filename)
{ 
    std::vector<int> machineCode;
    std::stringstream tempStream;
    std::ifstream ifile(filename);

    Segment codeSegment;
    Segment stackSegment;
    Segment dataSegment;

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
            //printf("%c", ch);
        }
    }
    else
    {
        printf("Failed to a program to launch: %s", strerror(errno));
        exit(1);
    }
    
    // Machine code is loaded to the list, now we load this code into RAM
    
    codeSegment = memcontroller.InitSegment(0);
    dataSegment = memcontroller.InitSegment(0);
    stackSegment = memcontroller.InitSegment(1);
    std::vector<int> pagesToIgnore;

    for(int i = 0; i < machineCode.size(); i++){
        if(i >= codeSegment.memory.addresses.size())
        {
            pagesToIgnore.insert(pagesToIgnore.end(), dataSegment.memory.usedPages.begin(),
            dataSegment.memory.usedPages.end());

            pagesToIgnore.insert(pagesToIgnore.end(), codeSegment.memory.usedPages.begin(),
            codeSegment.memory.usedPages.end());

            pagesToIgnore.insert(pagesToIgnore.end(), stackSegment.memory.usedPages.begin(),
            stackSegment.memory.usedPages.end());

            Memory newmem = memcontroller.AllocateMemory(machineCode.size() - codeSegment.memory.addresses.size(), pagesToIgnore);

            codeSegment.memory.usedPages.insert(codeSegment.memory.usedPages.end(),
            newmem.usedPages.begin(), newmem.usedPages.end());
            
            codeSegment.memory.addresses.insert(codeSegment.memory.addresses.end(),
            newmem.addresses.begin(), newmem.addresses.end());
        }
        memcontroller.WriteSegment(codeSegment, codeSegment.memory.addresses[i], machineCode[i]);
    }

    sp = stackSegment.startPointer + PAGE_SIZE-1;
    pc = 0;

    activeProgram = {dataSegment, codeSegment, stackSegment, {pc, 0, 0, 0, sp, 0, 0}};

    return activeProgram;
}

Program Cpu::ExecuteProgram(Program program, int cycles)
{
    int c = 0;
    SetFromSnapshot(program.cpuSnapshot);
    activeProgram = memcontroller.PrepareProgramMemory(program);
    sp = activeProgram.stackSegment.memory.addresses[activeProgram.stackSegment.memory.addresses.size()-1];
    
    while(c < cycles && (fs & ef) == 0)
    {
        try
        {
            Fetch();
            Decode();
        }
        catch(std::runtime_error &error)
        {
            throw error;
        }
        c++;
    }
    activeProgram.cpuSnapshot = SaveToSnapshot();
    if(!((fs & ef) == 0))
    {
        memcontroller.FreeMemory(activeProgram.codeSegment.memory);
        memcontroller.FreeMemory(activeProgram.stackSegment.memory);
        memcontroller.FreeMemory(activeProgram.dataSegment.memory);
    }

    return activeProgram;
}

CpuSnapshot Cpu::SaveToSnapshot()
{
    CpuSnapshot snap;
    snap.acc = acc;
    snap.addr = addr;
    snap.cReg = cReg;
    snap.fs = fs;
    snap.ir = ir;
    snap.pc = pc;
    snap.sp = sp;
    snap.xReg = xReg;
    snap.retReg = retReg;
    return snap;
}

void Cpu::SetFromSnapshot(CpuSnapshot snapshot)
{
    acc = snapshot.acc;
    addr = snapshot.addr;
    cReg = snapshot.cReg;
    fs = snapshot.fs;
    sp = snapshot.sp;
    ir = snapshot.ir;
    pc = snapshot.pc;
    xReg = snapshot.xReg;
}

int Cpu::AddInternal(int x, int y)
{
    if(x + y > std::numeric_limits<int>::max())
        fs |= of;
    return x + y;
}

int Cpu::MulInternal(int x, int y)
{
    if(x * y > std::numeric_limits<int>::max())
        fs |= of;
    return x * y;
}

bool Cpu::ParityCheck()
{
    bool parity = 0; 
    int n = acc;
    while (n) 
    { 
        parity = !parity; 
        n     = n & (n - 1); 
    }      
    return parity; 
}

// Interrupts
void Cpu::int10()
{
    HeapBlockHandler handle;
    for(auto i : HeapBlockHandlers)
    {
        if(i.start = xReg)
        {
            handle = i;
            xReg = 0;
            break;
        }
    }
    std::string str = memcontroller.ReadStringFromHeap(handle);
    printf("%s\n", str.c_str());
}

void Cpu::int3()
{
    HeapBlockHandler handle;
    for(auto i : HeapBlockHandlers)
    {
        if(i.start = cReg)
        {
            handle = i;
            break;
        }
    }
   // std::string str = memcontroller.ReadStringFromHeap(handle);
    //Create program
    //auto code = iocontroller.FindProgramCode(str);
    std::string str = "a";
    auto program = LoadProgram("kern");
    //
    acc = memcontroller.ForkProcess(str, program);
    cReg = 0;

    ExecuteProgram(program);
}