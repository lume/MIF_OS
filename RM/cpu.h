#pragma once 
#include <vector>
#include <array>

#define RAM_SIZE 32 

class Cpu
{
    public:
        Cpu(){}
        void ShowRam(); // Show the contents of the RAM
        void LoadProgram(std::vector<int> machineCode);// Load the Program machine code to the memory
        void ExecuteProgram(); // Execute the loaded program

    private:
        std::array<int, RAM_SIZE> RAM = {0};

        // Register definition
        uint16_t pc = 0x0;   // program counter
        uint16_t addr = 0x0; // internal addr register
        uint16_t acc = 0x0;   // accumulator
        uint16_t ir = 0x0;   // instruction register
        uint16_t sp = RAM_SIZE - 1; // stack pointer 
        uint16_t fs = 0x0; // flags

        void Fetch();
        void Decode();

        void OP_STOP();
        void OP_LOAD();
        void OP_STORE();
        void OP_ADD();
        void OP_SUB();
        void OP_JZ();
        void OP_JNZ();
        void OP_JL();
        void OP_JLE();
        void OP_JG();
        void OP_JGE();
        void OP_JMP();
        void OP_DIV();
        void OP_MOD();
        void OP_PUSH();
        void OP_POP();
        void OP_INC();
        void OP_DEC();
        void UNDEFINED();
};
