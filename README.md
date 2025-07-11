# RISC-V Simulator with Cache Simulation

This project implements a RISC-V Simulator with cache simulation, developed in the Computer Architecture course under Prof. Rajesh Kedia.

## Contents

This repository includes the following files:

#### 1. main.c
Contains the main source code for this project.

#### 2. run.c, run.h
Defines functions and their implementations to execute the given assembly code.

#### 3. stack.c, stack.h
Implements stack-related functions.

#### 4. functions.c, functions.h 
Provides utility functions for parsing, string operations, register lookup, etc.

#### 5. Makefile
A makefile to compile the C source files and generate an executable named `riscv_sim`.

#### 6. README.md
This file. Contains information about the project and usage instructions.

#### 7. report.pdf
Project report describing implementation details, challenges, error handling, assumptions, etc.

## Group Members

- [vhemanth06](https://github.com/vhemanth06)
- [thisissam-1706](https://github.com/thisissam-1706)

## Running the Project

To compile and run the project, follow these steps:

1. Clone the repository using:
    ```
    git clone https://github.com/vhemanth06/RISCV-Simulator.git
    ```
2. Navigate to the project folder:
    ```
    cd RISCV-Simulator
    ```
3. Add your input file containing RISC-V instructions to this folder.
4. Compile the code:
    ```
    make
    ```
5. Run the executable:
    ```
    ./riscv_sim
    ```
6. The RISC-V Simulator will start in the terminal. Use the commands below to interact with the simulator.

## Commands

- `load FILE_NAME`  
  Load an input file containing RISC-V assembly code.

- `run`  
  Execute the assembly code in the input file.

- `exit`  
  Exit the simulator.

- `step`  
  Execute a single instruction.

- `mem ADDRESS COUNT`  
  Display memory values from ADDRESS to ADDRESS+COUNT-1.

- `show-stack`  
  Show the call stack at the current moment.

- `break LINE_NUM`  
  Set a breakpoint at the given line number.

- `del break LINE_NUM`  
  Delete a breakpoint at the specified line. Returns an error if no breakpoint exists at that line.

- `cache_sim enable FILE_NAME`  
  Enable cache simulation with configurations from FILE_NAME.

- `cache_sim disable`  
  Disable cache simulation.

- `cache_sim status`  
  Display whether cache simulation is enabled or disabled.

- `cache_sim invalidate`  
  Invalidate all valid cache entries.

- `cache_sim dump FILE_NAME`  
  Write all current valid cache entries to FILE_NAME.

- `cache_sim stats`  
  Show cache statistics at the current program state.
---
