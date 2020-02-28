# Real machine document

## outline
- CPU and its ISA & Registers
- Memory and its segments
- I/O
- Clock and its functionality
- MEMCONTROL
- IOCONTROL
- Boot and operations
- CPU Scheduling


# CPU and its ISA & Registers

REGISTERS:

Accumulator [A] uint16_t  
[X] uint16_t  
[C] uint16_t  

IR (Instruction Register) uint16_t  
PC (Program Counter) uint16_t  
SP (Stack Pointer) uint16_t  
Flags {Zero, Sign, Carry, Debug, Lower}  

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

# CPU Scheduling

The CPU scheduling will rely on MLFQ Algorithm:

We'll have three queues:
Q1: RR with time quantum of 4
Q2: RR with time quantum of 7
Q3: RR with time quantum of 10

- Rule 1:If Priority(A)>Priority(B), A runs (B doesn’t).
- Rule 2:If Priority(A)=Priority(B), A & B run in round-robin fashion using the time slice (quantum length) of the given queue.
- Rule 3:When a job enters the system, it is placed at the highest priority (the topmost queue).
- Rule 4:Once a job uses up its time allotment at a given level (re-gardless of how many times it has given up the CPU), its priority is reduced (i.e., it moves down one queue).
- Rule 5:After some time periods, move all the jobs in the system to the topmost queue.

Some notes on Round Robin:
Split tasks into time slices, and switch between them. It improves the response time, but will worsen the turnaround time.

