# Lab4_AI23BTECH11033_AI23BTECH11025
The above file contains the code of a RISCV Simulator.
## Contents
The compressed zip file Lab4_AI23BTECH11033_AI23BTECH11025.zip contains the following files:
#### 1. main.c
This programs the the main source code of this project
#### 2. run.c run.h
These programs contains functions and their definations that help in implementing the given assembly code
#### 3. stack.c stack.h
These programs contains functions for implementation of a stack.
#### 4. functions.c functions.h 
These programs contains utility functions that are used for parcing,checking a char in string,finding a register from given input etc.
#### 5. Makefile
A makefile that would compile the C codes and generate an executable named riscv_sim.
#### 6. README.md
This file contains the information about this folder and usage instructions.
#### 7. report.pdf
This file contains the project report.It contains my implementation,challenges,error handling ,assumption etc.
#### 8. README.pdf
This file is the md to pdf converted file of README.md
## Running the project
to compile and run the project , follow the below steps:
#### 1. Download the Lab4_AI23BTECH11033_AI23BTECH11025.zip zip file
#### 2. Extract the folder into a directory of your choice
#### 3. Make sure to add the input.s file containing the instructions to this folder.
#### 4. Move to the Extracted folder in the directory in Terminal.
    cd Lab4_AI23BTECH11033_AI23BTECH11033
#### 5. compile the code by using the following command 
    make all
#### 6. Run the Executable file riscv_asm using the following command
    ./riscv_sim
#### 7.Hence a RISC-V Simulator is started in the terminal.use the following commands to use the simulator for RISCV assembly code implementation.
##### a.load FILE_NAME 
Used to load a input file containing riscv assembly code.
##### b.run
Used to run the assembly code present in input file.
##### c.exit
Used to exit the simulator.
##### d.step
Used to run a single instruction of input assembly code.
##### e.mem ADDRESS COUNT
Used to show the values of memory addresses from ADDRESS to ADDRESS+COUNT-1.
##### f.show-stack
Used to show the call stack at a certain instance of code implementation.
##### g.break LINE_NUM
Used to create a breakpoint at line having line number as LINE_NUM
##### h.del break LINE_NUM
Used to delete a breakpoint at line having line number as LINE_NUM.if line at LINE_NUM doesn't have a breakpoint,it returns a error.
