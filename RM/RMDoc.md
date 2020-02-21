# Real machine document

## outline
- CPU and its ISA & Registers
- Memory and its segments
- I/O
- Clock and its functionality
- MEMCONTROL
- IOCONTROL
- Boot and operations


# CPU and its ISA & Registers

REGISTERS:

Accumulator [A] uint16_t  
[X] uint16_t  
[C] uint16_t  

IR (Instruction Register) uint16_t  
PC (Program Counter) uint16_t  
SP (Stack Pointer) uint16_t  
Flags {Zero, Sign, Carry, Debug, Lower}  

maybe will be added:
TLB for hardware memory paging

ISA:  
LD [Addr]  
LD imm   
LD REG  
ST [Addr]  
ST REG  
ADD [Addr]  
Add imm  
ADD REG  
SUB [Addr]  
SUB imm  
SUB REG  
MUL [Addr]  
MUL imm  
MUL REG  
DIV [Addr]  
DIV imm  
DIV X_REG // Only possible to divide by X register, as after dot value is transfered to C  
JMP LABEL  
JZ LABEL  
JNZ LABEL  
JL LABEL  
JNL LABEL  
PUSH // Push the value stored in A into Stack    
POP  // Pop the value from the Stack into A  
INT imm // Call the interrupt imm (will ignore if interrupt immediate is incorrect)  
SHL imm // left shift value in A by imm  
SHR imm // right shift value in A by imm  
AND [Addr]  
AND imm  
AND REG  
OR [Addr]  
OR imm  
OR REG  
XOR [Addr]  
XOR imm  
XOR REG  
INC   
DEC  

*CMP A with...  Result stored in L flag*
CMP [Addr]  
CMP imm  
CMP REG  

# Memory and its segments

Planned RAM size: 65536 bytes (64K)  
CODE -> 40 K
DATA -> 8 K  
STACK -> 4 K  
RESERVED -> 12 K  

DISK:
10 MB
AVAILABLE TO USE -> 9 MB


# I/O

Core functionality of the I/O component is to perform operations of writing into disk, reading keyboard input.

# Clock and its functionality

The clock will send update signals to other components at 100 kHz (1/10 of the CPU frequency) rate.

# MEMCONTROL

This component is responsible for managing RAM read/write operations, memory segmentation.

By design, the behavior of this component is planned to mimic the Northbridge of the computer

# IOCONTROL

This component is responsible for sending keyboard input signals to the cpu and memory, and for splitting the 'disk' into sectors.

By design, the behavior of this component is planned to mimic the Southbridge of the computer.

Additionally, making this component control the swap is planned as well.

# Boot and operations

On boot, the empty system is planned to provide a primitive command prompt.