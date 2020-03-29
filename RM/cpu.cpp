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

//#define RAM memcontroller.RAM

//TODO: encapsulate RAM read/write operations 

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
    CALL,
    VAR,
    PTR,
    LOADV,
    LOADP,
    STOREP,
    STOREV
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

void Cpu::OP_LOADI()
{
    pc++;
    acc = RAM[pc];
    pc++;
}

void Cpu::OP_LOADR()
{
    pc++;
    char c = RAM[pc];
    
    if(c == 'x')
        acc = xReg;
    else if(c == 'c')
        acc = cReg;
   
    pc++;
}

void Cpu::OP_STOREA()
{
    pc++;
    addr = RAM[pc];
    RAM[addr] = acc;
    pc++;
}

void Cpu::OP_STORER()
{
    pc++;
    char c = RAM[pc];

    if(c == 'x')
        xReg = acc;
    else if(c == 'c')
        cReg = acc;

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

void Cpu::OP_ADDR()
{
    pc++;  
    char c = RAM[pc];

    if(c == 'x')
        acc += xReg;
    else if(c == 'c')
        acc += cReg;

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

void Cpu::OP_SUBI()
{
    pc++;
    acc += RAM[pc];

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_SUBR()
{
    pc++;
    char c = RAM[pc];

    if(c == 'x')
        acc -= xReg;
    else if(c == 'c')
        acc -= cReg;

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_MULA()
{
    pc++;
    addr = RAM[pc];
    acc *= RAM[addr];

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_MULI()
{
    pc++;
    acc *= RAM[pc];
    
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_MULR()
{
    pc++;
    char c = RAM[pc];

    if(c == 'x')
        acc *= xReg;
    else if(c == 'c')
        acc *= cReg;
    
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
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

void Cpu::OP_DIVI()
{
    pc++;
    acc = acc / RAM[pc];

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_DIVR()
{
    pc++;
    char c = RAM[pc];

    if(c == 'x')
        acc /= xReg;
    else if(c == 'c')
        acc /= cReg;
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
        uint8_t offset = RAM[pc];
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
        uint8_t offset = RAM[pc];
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
        uint8_t offset = RAM[pc];
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
        uint8_t offset = RAM[pc];
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
        uint8_t offset = RAM[pc];
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
        uint8_t offset = RAM[pc];
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
    uint8_t offset = RAM[pc];
    pc++;
    pc += offset;
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

void Cpu::OP_INT()
{
   //TODO: implement interrupts
}

void Cpu::OP_ANDA()
{
    pc++;
    addr = RAM[pc];
    acc = acc & RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_ANDI()
{
    pc++;
    acc = acc & RAM[pc];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_ANDR()
{
    pc++;
    char c = RAM[pc];

    if(c == 'x')
        xReg &= acc;
    else if(c == 'c')
        cReg &= acc;

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_ORA()
{
    pc++;
    addr = RAM[pc];
    acc = acc | RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_ORI()
{
    pc++;
    acc = acc | RAM[pc];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_ORR()
{
    pc++;
    char c = RAM[pc];

    if(c == 'x')
        xReg |= acc;
    else if(c == 'c')
        cReg |= acc;

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_XORA()
{
    pc++;
    addr = RAM[pc];
    acc = acc ^ RAM[addr];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_XORI()
{
    pc++;
    acc = acc ^ RAM[pc];
    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_XORR()
{
    pc++;
    char c = RAM[pc];

    if(c == 'x')
        xReg ^= acc;
    else if(c == 'c')
        cReg ^= acc;

    if(acc < 0)
        fs |= sf;
    if(acc == 0)
        fs |= zf;
    pc++;
}

void Cpu::OP_CMPA()
{
    pc++;
    addr = RAM[pc];
    uint16_t val = RAM[addr];
    
    if(acc < val)
        fs |= lf;
    if(acc == val)
        fs |= zf;
    pc++;
}

void Cpu::OP_CMPI()
{
    pc++;
    uint16_t val = RAM[pc];
    
    if(acc < val)
        fs |= lf;
    if(acc == val)
        fs |= zf;
    pc++;
}

void Cpu::OP_CMPR()
{
    pc++;
    char c = RAM[pc];
    uint16_t val;
    
    if(c == 'x')
        val = xReg;
    else if(c == 'c')
        val = cReg;

    if(acc < val)
        fs |= lf;
    if(acc == val)
        fs |= zf;
    pc++;
}

void Cpu::OP_CALL()
{
    pc++;
    //TODO: add overflow check
    uint16_t offset = RAM[pc];
    pc++;
    pc += offset;
}

void Cpu::OP_SHR()
{
    pc++;
    acc >> RAM[pc];
    pc++;
}

void Cpu::OP_SHL()
{
    pc++;
    acc << RAM[pc];
    pc++;
}

void Cpu::OP_VAR()
{
    pc++;
    int x = RAM[pc];
    if(memcontroller.CheckIfVarExists(activeProgram, x))
    {
        //TODO: implement raising exception that won't kill everyting when developing an OS
        throw new std::runtime_error("Such variable already exists");
    }
    int varAddr = activeProgram.dataSegment.writePointer;
    RAM[varAddr] = RAM[pc];
    RAM[varAddr+1] = RAM[pc];
    pc++;
    activeProgram.dataSegment.writePointer += 2; 
}

void Cpu::OP_PTR()
{
    //TODO: Implement
    pc++;
    pc++;
}

void Cpu::OP_LOADP()
{
    //TODO: Implement
    pc++;
    pc++;
}

void Cpu::OP_LOADV()
{
    pc++;
    int addr = memcontroller.FindVarAddress(activeProgram, RAM[pc]);
    acc = RAM[addr+1];
    pc++;
}

void Cpu::OP_STOREV()
{
    pc++;
    int addr = memcontroller.FindVarAddress(activeProgram, RAM[pc]);
    RAM[addr+1] = acc;
    pc++;
}

void Cpu::OP_STOREP()
{
    //TODO: Implement
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
    default:
        UNDEFINED();
        break;
    }
}

// public functions

void Cpu::ShowRam()
{
    printf("CODE:\n");
    printf("[");
    for(int i = 4096; i < 4197; i++)
    {
        printf("%d, ", RAM[i]);
    }
    printf("]\n");

    printf("STEK:\n");
    printf("[");
    for(int i = 12187; i < 12288; i++)
    {
        printf("%d, ", RAM[i]);
    }
    printf("]\n");

    printf("DATA:\n");
    printf("[");
    for(int i = 0; i < 101; i++)
    {
        printf("%d, ", RAM[i]);
    }
    printf("]\n");

    if(zf)
        printf("zero flag is set!\n");
    if(sf)
        printf("sign flag is set!\n");
}

Program Cpu::LoadProgram(std::string filename)
{ 
    //TODO: Rework program loading so it handles virtual memory correctly
    //For now we do not allow to have programs bigger than our RAM
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
    dataSegment = memcontroller.InitSegment(0);
    codeSegment = memcontroller.InitSegment(0);
    stackSegment = memcontroller.InitSegment(1);

    for(int i = 0; i < machineCode.size(); i++){
        memcontroller.WriteSegment(codeSegment, codeSegment.memory.addresses[i], machineCode[i]);
    }

    // Map system registers to segments
    sp = stackSegment.startPointer+4095;
    pc = codeSegment.startPointer;

    activeProgram = {dataSegment, codeSegment, stackSegment, SaveToSnapshot()};

    return activeProgram;
}

void Cpu::ExecuteProgram(Program program)
{
    SetFromSnapshot(program.cpuSnapshot);
    activeProgram = program;
    int i = RAM[pc];
    while(i != 0)
    {
        Fetch();
        Decode();
        i = RAM[pc];
    }
    program.cpuSnapshot = SaveToSnapshot();
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
