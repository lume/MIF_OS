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
TODO:

# Memory and its segments
TODO:

# I/O

Core functionality of the I/O component is to perform operations of writing into disk, reading keyboard input.

# Clock and its functionality

The clock will send update signals to other components at 100 kHz (1/10 of the CPU frequency) rate.

# MEMCONTROL

This component is responsible for managing RAM read/write operations, memory segmentation. TODO:

By design, the behavior of this component is planned to mimic the Northbridge of the computer

# IOCONTROL

This component is responsible for sending keyboard input signals to the cpu and memory, and for splitting the 'disk' into sectors.

By design, the behavior of this component is planned to mimic the Southbridge of the computer.

# Boot and operations

On boot, the empty system is planned to provide a primitive command prompt.